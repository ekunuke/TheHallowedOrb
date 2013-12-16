#include "Pedestal.h"

Pedestal::Pedestal(Level& level, float x, float y):
    Entity(level, TYPE_PEDESTAL, x, y, 6, 16),
    sprite(ResourceManager::getTexture("pedestal.png"), 6, 16, -1, 0, 0, 0, false)
{
    sprite.setOrigin(0, 0);
}

Pedestal::~Pedestal()
{
    //dtor
}


void Pedestal::draw(sf::RenderWindow& window)
{
    sprite.setPosition(x, y);
    sprite.draw(window);
}
