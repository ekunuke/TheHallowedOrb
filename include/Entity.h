#ifndef ENTITY_H
#define ENTITY_H

#include "toolbox.h"
#include "Level.h"

class Entity
{
    public:
        static const float GRAVITY;
        enum EntityType
        {
            TYPE_PLAYER,
            TYPE_PEDESTAL,
            TYPE_ZOMBIE,
            TYPE_LUNGER,
            TYPE_FLYER,
            TYPE_SUICIDER,
            TYPE_PROJECTILE,
            TYPE_BOUNCY,
            TYPE_PARTICLE,
            TYPE_BOSS,
            TYPE_POWERUP
        };
        Entity(Level& level, EntityType entityType, float x, float y, float w, float h);
        virtual ~Entity();

        virtual void step();
        virtual void draw(sf::RenderWindow& window);
        virtual void collideEntity(Entity* other);
        bool isOutOfBounds();
        bool isCollidingWith(Entity* other);
        void separateFromEntity(Entity* other);
        void drawBoundingBox(sf::RenderWindow& window);


        void drawShield(sf::RenderWindow& window);
    protected:
        void collideLevel();

        Level& level;
    public:
        EntityType entityType;
        float x;
        float y;

        float dx;
        float dy;

        float w;
        float h;

        bool shield;
        bool enemy;
        bool landedDown;
        bool landedLeft;
        bool landedRight;
        bool landedUp;
        bool palpable;
        bool die;
};

#endif // ENTITY_H
