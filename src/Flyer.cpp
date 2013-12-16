#include "Flyer.h"
#include "Player.h"

const float Flyer::WIDTH = 10;
const float Flyer::HEIGHT = 10;

const float Flyer::ACCEL_RATE = 0.01;
const float Flyer::MAX_SPEED = 1;

Flyer::Flyer(Level& level, float x, float y):
    Entity(level, TYPE_FLYER, x, y, WIDTH, HEIGHT),
    sprite(ResourceManager::getTexture("eye.png"), 16, 16, -1, 12, 0, 0, true)
{
    enemy = true;
}

Flyer::~Flyer()
{
    //dtor
}

void Flyer::step()
{
    Player* player = level.getPlayer();
    float distX = player->x + player->w/2 - (x + w/2);
    float distY = player->y + player->h/2 - (y + h/2);

    float magnitude = sqrtf(distX * distX + distY * distY);
    dx += ACCEL_RATE * (distX / magnitude);
    dy += ACCEL_RATE * (distY / magnitude);
    magnitude = sqrtf(dx * dx + dy * dy);
    if(magnitude > MAX_SPEED)
    {
        dx *= MAX_SPEED / magnitude;
        dy *= MAX_SPEED / magnitude;
    }

    x += dx;
    y += dy;

    collideLevel();
    sprite.step();
}

void Flyer::draw(sf::RenderWindow& window)
{
    sprite.setPosition(x + w/2, y + h/2);
    sprite.draw(window);
    //drawBoundingBox(window);
}


void Flyer::collideEntity(Entity* other)
{
    if(other->entityType == TYPE_FLYER)
    {
        separateFromEntity(other);
    }
}
