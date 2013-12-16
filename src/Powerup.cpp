#include "Powerup.h"

const float Powerup::WIDTH = 7;
const float Powerup::HEIGHT = 7;
const int Powerup::LIFE_TIME = 60 * 16; // 10 seconds to get it
Powerup::Powerup(Level& level, float x, float y):
    Entity(level, TYPE_POWERUP, x, y, WIDTH, HEIGHT),
    sprite(ResourceManager::getTexture("powerup_health.png"), 7, 7, -1, 0, 0, 0, false),
    frame(0),
    lifeTimer(0)
{
    //ctor
}

Powerup::~Powerup()
{
    //dtor
}

void Powerup::step()
{
    frame++;
    lifeTimer++;
    if(lifeTimer >= LIFE_TIME)
    {
        die = true;
    }

    x += dx;
    y += dy;
    collideLevel();
}

void Powerup::draw(sf::RenderWindow& window)
{
    if(lifeTimer > LIFE_TIME / 2)
    {
        if(lifeTimer % 30 < 5)
        {
            return;
        }
    }
    sprite.setPosition(x+w/2, y+h/2 + cosf(frame/20.f) * 2 - 2);
    sprite.draw(window);
}
