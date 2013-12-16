#ifndef PEDESTAL_H
#define PEDESTAL_H

#include "Entity.h"
#include "Sprite.h"

class Pedestal : public Entity
{
    public:
        Pedestal(Level& level, float x, float y);
        virtual ~Pedestal();

        virtual void draw(sf::RenderWindow& window);
    protected:
    private:
        Sprite sprite;
};

#endif // PEDESTAL_H
