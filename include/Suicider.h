#ifndef SUICIDER_H
#define SUICIDER_H

#include "Entity.h"
#include "Sprite.h"

class Suicider : public Entity
{
    public:
        static const float WIDTH;
        static const float HEIGHT;
        static const float EXPLOSION_WIDTH;
        static const float EXPLOSION_HEIGHT;
        static const float ACCEL_RATE;
        static const float MAX_SPEED;
        static const float EXPLODE_RANGE;
        static const int EXPLODE_TIME;
        Suicider(Level& level, float x, float y);
        virtual ~Suicider();

        virtual void step();
        virtual void draw(sf::RenderWindow& window);
        virtual void collideEntity(Entity* other);

    protected:
    private:
        void transformIntoExplosion();
        Sprite sprite;
        bool exploding;
        int explodeTimer;
};

#endif // SUICIDER_H
