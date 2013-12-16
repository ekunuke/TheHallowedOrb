#ifndef PARTICLE_H
#define PARTICLE_H

#include "Entity.h"

class Particle : public Entity
{
    public:
        enum ParticleType
        {
            PARTICLE_BLOOD,
            PARTICLE_FIRE,
            PARTICLE_BLUE,
            PARTICLE_DEEP_BLUE,
            PARTICLE_YELLOW,
            PARTICLE_RED,
            PARTICLE_HEALTH,
            PARTICLE_EXPLOSION
        };
        Particle(Level& level, float x, float y, ParticleType pType);
        virtual ~Particle();

        virtual void step();
        virtual void draw(sf::RenderWindow& window);
    private:
        ParticleType particleType;

        int lifeTimer;
        int maxLife;
        float gravity;
};

#endif // PARTICLE_H
