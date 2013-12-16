#ifndef BOUNCY_H
#define BOUNCY_H

#include "Entity.h"
#include "Sprite.h"

class Bouncy : public Entity
{
    public:
        static const float WIDTH;
        static const float HEIGHT;
        Bouncy(Level& level, float x, float y);
        virtual ~Bouncy();
        virtual void step();
        virtual void draw(sf::RenderWindow& window);
        virtual void collideEntity(Entity* other);
    protected:
    private:
        Sprite sprite;
        int showBounceTimer;
};

#endif // BOUNCY_H
