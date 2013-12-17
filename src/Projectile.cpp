#include "Projectile.h"
#include "Particle.h"
#include "ResourceManager.h"
#include "Boss.h"
#include "Player.h"

const int Projectile::WIDTH = 6;
const int Projectile::HEIGHT = 6;
const int Projectile::FREEZE_TIME = 60;

Projectile::Projectile(Level& level, float x, float y, float ndx, float ndy):
    Entity(level, TYPE_PROJECTILE, x, y, WIDTH, HEIGHT),
    stuck(false),
    onPedestal(false),
    ragnarok(false),
    freezeTimer(FREEZE_TIME),
    scale(1.0),
    sprite(ResourceManager::getTexture("projectile.png"), 6, 6, -1, 0, 0, 0, false),
    angle(0),
    combo(0),
    returnToPlayer(false)
{
    dx = ndx;
    dy = ndy;
}

Projectile::~Projectile()
{
    //dtor
}

void Projectile::finishCombo()
{
    if(combo > 0)
    {
        if(combo > 2) /// test
        {
            returnToPlayer = true;
        }
    }
    combo = 0;
}

void Projectile::step()
{
    if(ragnarok)
    {
        if(freezeTimer > 0)
        {
            freezeTimer--;
            return;
        }
        if(scale < 40)
        {
            scale *= 1.1;
            return;
        }
        explode();
        return;
    }

    if(!onPedestal)
    {
        if(landedDown)
        {
            finishCombo();
            dx *= 0.85;
        }
        else
        {
            if(rand()%2 == 0)
            {
                level.addEntity(new Particle(level, x, y, Particle::PARTICLE_BLUE));
            }
            if(combo > 0)
            {
                if(rand()%2 == 0)
                {
                    level.addEntity(new Particle(level, x + rand()%(int)(w), y + h + 1, Particle::PARTICLE_DEEP_BLUE));
                }
            }
            if(combo > 1)
            {
                if(rand()%2 == 0)
                {
                    level.addEntity(new Particle(level, x + rand()%(int)(w), y + h + 1, Particle::PARTICLE_YELLOW));
                }
            }
            if(combo > 2)
            {
                if(rand()%2 == 0)
                {
                    level.addEntity(new Particle(level, x + rand()%(int)(w), y + h + 1, Particle::PARTICLE_RED));
                }
            }
        }
        if(returnToPlayer)
        {
            Player* player = level.getPlayer();
            float distX = player->x+w/2 - (x + w/2);
            float distY = player->y+h/2 - (y + h/2);
            //float mag = sqrtf(distX * distX + distY * distY);
            if(returnToPlayer)
            {
                x += distX / 40;
                y += distY / 40;
                dx += distX / 40;
                dy += distY / 40;
            }
        }

        dy += GRAVITY;
        x += dx;
        y += dy;
        angle += dx * 12;
    }
    else
    {
        if(rand()%2 == 0)
        {
            level.addEntity(new Particle(level, x + rand()%(int)(w), y + h + 1, Particle::PARTICLE_BLUE));
        }
    }
    float prevDx = dx;
    collideLevel();
    if(landedLeft || landedRight)
    {
        dx = prevDx * -0.9;
    }
    if(landedDown || landedLeft || landedRight || landedUp)
    {
        stuck = true;
    }
}

void Projectile::draw(sf::RenderWindow& window)
{
    if(ragnarok)
    {
        sprite.setScale(scale);
    }
    sprite.setPosition(x+w/2, y+h/2);
    sprite.setRotation(angle);
    sprite.draw(window);
}


void Projectile::collideEntity(Entity* other)
{
    if(!onPedestal && !landedDown && (other->enemy))
    {
        bool hit = false;
        other->dx += dx;
        other->dy += dy;
        stuck = true;
        dx *= -1;
        dy *= -1;
        level.shake(5, 3 * (combo+1));
        if(other->shield)
        {
            hit = true;
            other->shield = false;
            combo++;
        }
        else
        {
            if(other->entityType == TYPE_BOSS)
            {
                if(static_cast<Boss*>(other)->hurt())
                {
                    hit = true;
                    for(unsigned i = 0; i < 20; i++)
                    {
                        level.addEntity(new Particle(level, x, y, Particle::PARTICLE_BLOOD));
                    }
                }
                else
                {
                    dx = 0;
                    dy = 0;
                }
                if(other->die)
                {
                    ragnarok = true;
                }
            }
            else
            {
                hit = true;
                combo++;
                other->die = true;
                for(unsigned i = 0; i < 20; i++)
                {
                    level.addEntity(new Particle(level, x, y, Particle::PARTICLE_BLOOD));
                }
            }
        }
        if(hit)
        {
            ResourceManager::playSound("projectile_hit.wav");
        }

    }
}

void Projectile::explode()
{
    ResourceManager::playSound("big_explosion.wav");
    die = true;
    level.shake(340);
    for(unsigned i = 0; i < 100; i++)
    {
        level.addEntity(new Particle(level, x + rand()%100 - 50, y + rand()%100 - 50, Particle::PARTICLE_EXPLOSION));
        level.addEntity(new Particle(level, x + rand()%100 - 50, y + rand()%100 - 50, Particle::PARTICLE_FIRE));
    }
}
