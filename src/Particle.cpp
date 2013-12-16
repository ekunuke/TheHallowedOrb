#include "Particle.h"

Particle::Particle(Level& level, float x, float y, ParticleType pType):
    Entity(level, TYPE_PARTICLE, x, y, 2, 2),
    particleType(pType),
    lifeTimer(100),
    gravity(0)
{
    palpable = false;
    switch(particleType)
    {
    case PARTICLE_BLOOD:
        maxLife = 120 + rand()%20;
        dy = -3 * (rand()/(float)RAND_MAX);
        dx = (rand()/(float)RAND_MAX) * 2 - 1;
        gravity = GRAVITY;
        break;
    case PARTICLE_FIRE:
        maxLife = 30 + rand()%50;
        gravity = -0.01;
        dy = (rand()/(float)RAND_MAX) - 0.5;
        dx = (rand()/(float)RAND_MAX) * 2 - 1;
        w = 3;
        h = 3;
        break;
    case PARTICLE_BLUE:
    case PARTICLE_DEEP_BLUE:
    case PARTICLE_YELLOW:
    case PARTICLE_RED:
        maxLife = 10 + rand()%30;
        gravity = 0;
        dy = (rand()/(float)RAND_MAX) - 0.5;
        dx = (rand()/(float)RAND_MAX) - 0.5;
        w = 2;
        h = 2;
        break;
    case PARTICLE_HEALTH:
        maxLife = 20 + rand()%30;
        gravity = 0;
        dy = (rand()/(float)RAND_MAX) - 0.5;
        dx = (rand()/(float)RAND_MAX) - 0.5;
        w = 3;
        h = 1;
        break;
    case PARTICLE_EXPLOSION:
        maxLife = 300 + rand()%30;
        gravity = 0;
        dy = (rand()/(float)RAND_MAX) - 0.5;
        dx = (rand()/(float)RAND_MAX) - 0.5;
        w = 200;
        h = 200;
        break;
    }
    lifeTimer = maxLife;
}

Particle::~Particle()
{
    //dtor
}


void Particle::step()
{
    if(lifeTimer > 0)
    {
        lifeTimer--;
        if(lifeTimer <= 0)
        {
            die = true;
        }
    }

    dy += gravity;
    x += dx;
    y += dy;
    if(particleType != PARTICLE_BLUE)
    {
        collideLevel();
    }
    if(landedDown || landedRight || landedLeft)
    {
        dx = 0;
        dy = 0;
    }
}

void Particle::draw(sf::RenderWindow& window)
{
    sf::RectangleShape box({w, h});
    box.setPosition(x, y);
    float lifePercent = lifeTimer / (float)maxLife;
    if(particleType == PARTICLE_BLOOD)
    {
        box.setFillColor(sf::Color(208, 70, 72));
    }
    else if(particleType == PARTICLE_FIRE)
    {
        box.setFillColor(sf::Color(255 , 255 * (lifePercent), 0, 255 * lifePercent));
    }
    else if(particleType == PARTICLE_BLUE)
    {
        box.setFillColor(sf::Color(109, 194, 202, 255 * lifePercent));
    }
    else if(particleType == PARTICLE_DEEP_BLUE)
    {
        box.setFillColor(sf::Color(89, 125, 206, 255 * lifePercent));
    }
    else if(particleType == PARTICLE_YELLOW)
    {
        box.setFillColor(sf::Color(245, 105, 4, 255 * lifePercent));
    }
    else if(particleType == PARTICLE_RED)
    {
        box.setFillColor(sf::Color(208, 70, 72, 255 * lifePercent));
    }
    else if(particleType == PARTICLE_HEALTH)
    {
        box.move(-1, 0);
        box.setFillColor(sf::Color(255, 0, 0, 255 * lifePercent));
        window.draw(box);
        box.move(1, -1);
        box.setSize({1, 3});
    }
    else if(particleType == PARTICLE_EXPLOSION)
    {
        box.setRotation(rand()%360);
        box.setFillColor(sf::Color(255, 255, 255, 255 * lifePercent));
    }

    window.draw(box);

}
