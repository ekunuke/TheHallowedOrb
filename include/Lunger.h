#ifndef LUNGER_H
#define LUNGER_H

#include "Entity.h"
#include "Sprite.h"

class Lunger : public Entity
{
    public:
        static const float WIDTH;
        static const float HEIGHT;
        static const float ACCEL_RATE;
        static const float MOVE_SPEED;
        static const float LUNGE_SPEED;
        Lunger(Level& level, float x, float y);
        virtual ~Lunger();

        virtual void step();
        virtual void draw(sf::RenderWindow& window);
        virtual void collideEntity(Entity* other);
    protected:
    private:
        void tryLunge();
        void tryJump();

        Sprite walkSprite;
        Sprite lungeSprite;
        Sprite* currSprite;

        bool lunging;

        bool movingRight;
        bool haveDirection;
};

#endif // LUNGER_H
