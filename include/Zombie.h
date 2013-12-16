#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Entity.h"
#include "Sprite.h"

class Zombie : public Entity
{
    public:
        static const float WIDTH;
        static const float HEIGHT;
        static const float ACCEL_RATE;
        static const float MOVE_SPEED;
        Zombie(Level& level, float x, float y);
        virtual ~Zombie();

        virtual void step();
        virtual void draw(sf::RenderWindow& window);
        virtual void collideEntity(Entity* other);
    protected:
    private:
        void tryJump();
        Sprite sprite;
        bool movingRight;
        bool haveDirection;
};

#endif // ZOMBIE_H
