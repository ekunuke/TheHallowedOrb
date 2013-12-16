#ifndef POWERUP_H
#define POWERUP_H

#include "Entity.h"
#include "Sprite.h"

class Powerup : public Entity
{
    public:
        static const float WIDTH;
        static const float HEIGHT;
        static const int LIFE_TIME;
        Powerup(Level& level, float x, float y);
        virtual ~Powerup();

        virtual void step();
        virtual void draw(sf::RenderWindow& window);
    protected:
    private:
        Sprite sprite;
        int frame;
        int lifeTimer;
};

#endif // POWERUP_H
