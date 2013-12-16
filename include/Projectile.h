#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Entity.h"
#include "Sprite.h"

class Projectile : public Entity
{
    public:
        static const int WIDTH;
        static const int HEIGHT;
        static const int FREEZE_TIME;
        Projectile(Level& level, float x, float y, float ndx, float ndy);
        virtual ~Projectile();

        virtual void step();
        virtual void draw(sf::RenderWindow& window);
        virtual void collideEntity(Entity* other);

        bool stuck;
        bool onPedestal;
        bool ragnarok;
        void finishCombo();
    protected:
    private:
        void explode();
        int freezeTimer;
        float scale;
        Sprite sprite;
        float angle;
        int combo;
        bool returnToPlayer;
        bool launchToPlayer;

};

#endif // PROJECTILE_H
