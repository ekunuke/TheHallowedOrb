#include "Bouncy.h"

const float Bouncy::WIDTH = Tile::TILE_SIZE;
const float Bouncy::HEIGHT = Tile::TILE_SIZE/2;


Bouncy::Bouncy(Level& level, float x, float y):
    Entity(level, TYPE_BOUNCY, x, y + Tile::TILE_SIZE/2, WIDTH, HEIGHT),
    sprite(ResourceManager::getTexture("bouncy.png"), 16, 16, -1, 0, 0, 8, false),
    showBounceTimer(0)
{
    sprite.setOrigin(0, 0);
    //ctor
}

Bouncy::~Bouncy()
{
}

void Bouncy::step()
{
    if(showBounceTimer > 0)
    {
        showBounceTimer--;
    }
}
void Bouncy::draw(sf::RenderWindow &window)
{
    if(showBounceTimer > 0)
    {
        sprite.setFrame(1);
    }
    else
    {
        sprite.setFrame(0);
    }
    sprite.setPosition(x, y);
    sprite.draw(window);
    //drawBoundingBox(window);
}

void Bouncy::collideEntity(Entity* other)
{
    if(other->entityType == TYPE_BOSS)
    {
        return;
    }
    if(other->dy >= 0)
    {
        ResourceManager::playSound("bounce.wav");
    }
    other->dy = -6;
    showBounceTimer = 10;
}
