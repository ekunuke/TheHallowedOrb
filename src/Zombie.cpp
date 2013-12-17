#include "Zombie.h"
#include "Player.h"

const float Zombie::WIDTH = 6;
const float Zombie::HEIGHT = 13;
const float Zombie::ACCEL_RATE = 0.1;
const float Zombie::MOVE_SPEED = 1.1;

Zombie::Zombie(Level& level, float x, float y):
    Entity(level, TYPE_ZOMBIE, x, y, WIDTH, HEIGHT),
    sprite(ResourceManager::getTexture("ugly.png"), 16, 16, -1, 6, 0, 0, true),
    movingRight(true),
    haveDirection(false)
{
    enemy = true;
    sprite.randomizeFrame();
}

Zombie::~Zombie()
{
    //dtor
}

void Zombie::step()
{
    if(!haveDirection)
    {
        Player* player = level.getPlayer();

        movingRight = (player->x + player->w/2) > (x + w/2);
        haveDirection = true;
    }
    //if(landedDown)
    {
        if(movingRight)
        {
            dx += ACCEL_RATE;
            if(dx > MOVE_SPEED)
            {
                dx = MOVE_SPEED;
            }
        }
        else
        {
            dx -= ACCEL_RATE;
            if(dx < -MOVE_SPEED)
            {
                dx = -MOVE_SPEED;
            }
        }
    }

    dy += GRAVITY;
    x += dx;
    y += dy;
    collideLevel();

    if(landedLeft || landedRight)
    {
        tryJump();
        haveDirection = false;
    }
    sprite.setFlipped(!movingRight);
    sprite.step();
}

void Zombie::draw(sf::RenderWindow& window)
{
    sprite.setPosition(x+w/2, y+h/2);
    sprite.draw(window);
    //drawBoundingBox(window);

}

void Zombie::collideEntity(Entity* other)
{
    if(other->entityType == TYPE_ZOMBIE || other->entityType == TYPE_LUNGER)
    {
        separateFromEntity(other);
        if(rand()%20 == 0)
        {
            haveDirection = false;
        }
    }

}

void Zombie::tryJump()
{
    if(landedDown)
    {
        dy = -3.2;
    }
}
