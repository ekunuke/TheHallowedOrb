#include "Suicider.h"
#include "Player.h"
#include "Particle.h"

const float Suicider::WIDTH = 10;
const float Suicider::HEIGHT = 10;
const float Suicider::EXPLOSION_WIDTH = 32;
const float Suicider::EXPLOSION_HEIGHT = 32;

const float Suicider::ACCEL_RATE = 0.02;
const float Suicider::MAX_SPEED = 2;
const float Suicider::EXPLODE_RANGE = 20;
const int Suicider::EXPLODE_TIME = 60;

Suicider::Suicider(Level& level, float x, float y):
    Entity(level, TYPE_SUICIDER, x, y, WIDTH, HEIGHT),
    sprite(ResourceManager::getTexture("bomber.png"), 16, 16, -1, 12, 0, 0, true),
    exploding(false),
    explodeTimer(EXPLODE_TIME)
{
    enemy = true;
}

Suicider::~Suicider()
{
    //dtor
}

void Suicider::step()
{
    if(exploding)
    {
        dx = 0;
        dy = 0;
        if(explodeTimer > 0)
        {
            explodeTimer--;
            if(explodeTimer == 1)
            {
                transformIntoExplosion();
            }
            else if(explodeTimer == 0)
            {
                die = true;
            }
        }
    }
    else
    {
        Player* player = level.getPlayer();
        float distX = player->x + player->w/2 - (x + w/2);
        float distY = player->y + player->h/2 - (y + h/2);

        float magnitude = sqrtf(distX * distX + distY * distY);
        if(magnitude <= EXPLODE_RANGE)
        {
            exploding = true;
        }
        else
        {
            dx += ACCEL_RATE * (distX / magnitude);
            dy += ACCEL_RATE * (distY / magnitude);
            magnitude = sqrtf(dx * dx + dy * dy);
            if(magnitude > MAX_SPEED)
            {
                dx *= MAX_SPEED / magnitude;
                dy *= MAX_SPEED / magnitude;
            }
        }
    }
    x += dx;
    y += dy;

    collideLevel();
    sprite.step();
}

void Suicider::draw(sf::RenderWindow& window)
{
    if(exploding)
    {
        sprite.setScale(1 + rand()/(float)RAND_MAX);
    }
    sprite.setPosition(x + w/2, y + h/2);
    sprite.draw(window);

   // drawBoundingBox(window);
}


void Suicider::collideEntity(Entity* other)
{
    if(other->entityType == TYPE_FLYER)
    {
        separateFromEntity(other);
    }
}

void Suicider::transformIntoExplosion()
{
    ResourceManager::playSound("explode.wav");
    x -= (EXPLOSION_WIDTH - WIDTH)/2;
    y -= (EXPLOSION_HEIGHT - HEIGHT)/2;
    w = EXPLOSION_WIDTH;
    h = EXPLOSION_HEIGHT;
    for(unsigned i = 0; i < 40; i++)
    {
        level.addEntity(new Particle(level, x + rand()%(int)(w), y + rand()%(int)(h), Particle::PARTICLE_FIRE));
    }
}
