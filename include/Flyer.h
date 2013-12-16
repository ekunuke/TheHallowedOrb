#ifndef FLYER_H
#define FLYER_H

#include "Entity.h"
#include "Sprite.h"

class Flyer : public Entity
{
    public:
        static const float WIDTH;
        static const float HEIGHT;
        static const float ACCEL_RATE;
        static const float MAX_SPEED;

        Flyer(Level& level, float x, float y);
        virtual ~Flyer();

        virtual void step();
        virtual void draw(sf::RenderWindow& window);
        virtual void collideEntity(Entity* other);

    protected:
    private:
        Sprite sprite;
};

#endif // FLYER_H
