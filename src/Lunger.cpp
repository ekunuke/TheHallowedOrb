#include "Lunger.h"
#include "Player.h"

const float Lunger::WIDTH = 6;
const float Lunger::HEIGHT = 13;
const float Lunger::ACCEL_RATE = 0.1;
const float Lunger::MOVE_SPEED = 1;
const float Lunger::LUNGE_SPEED = 3;

Lunger::Lunger(Level& level, float x, float y):
    Entity(level, TYPE_LUNGER, x, y, WIDTH, HEIGHT),
    walkSprite(ResourceManager::getTexture("lunger.png"), 16, 16, -1, 6, 0, 0, true),
    lungeSprite(ResourceManager::getTexture("lunger_attack.png"), 16, 16, -1, 11, 0, 0, false),
    currSprite(&walkSprite),
    lunging(false),
    movingRight(true),
    haveDirection(false)
{
    enemy = true;
    walkSprite.randomizeFrame();
}

Lunger::~Lunger()
{
    //dtor
}

void Lunger::step()
{
    Player* player = level.getPlayer();

    if(!haveDirection)
    {

        movingRight = (player->x + player->w/2) > (x + w/2);
        haveDirection = true;
    }
    if(landedDown)
    {
        float xDist = player->x + player->w/2 - (x + w/2);
        float yDist = player->y + player->h/2 - (y + h/2);
        bool facingPlayer = movingRight == ((player->x + player->w/2) > (x + w/2));
        if(player->landedDown &&
           facingPlayer &&
           fabs(yDist) < Tile::TILE_SIZE &&
           fabs(xDist) < 3 * Tile::TILE_SIZE)
        {
            tryLunge();
        }
    }
    if(!lunging)
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
    else
    {
        dx *= 0.95;
        if(fabs(dx) < 0.1)
        {
            lunging = false;
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
    currSprite->setFlipped(!movingRight);
    currSprite->step();
}

void Lunger::draw(sf::RenderWindow& window)
{
    if(lunging)
    {
        currSprite = &lungeSprite;
    }
    else
    {
        currSprite = &walkSprite;
    }
    currSprite->setPosition(x+w/2, y+h/2);
    currSprite->draw(window);
}

void Lunger::collideEntity(Entity* other)
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

void Lunger::tryLunge()
{
    if(lunging)
    {
        return;
    }
    lunging = true;
    lungeSprite.reset();
    dx = LUNGE_SPEED * (movingRight ? 1 : -1);
    dy = -1.2;
}

void Lunger::tryJump()
{
    if(landedDown)
    {
        dy = -3.2;
    }
}
