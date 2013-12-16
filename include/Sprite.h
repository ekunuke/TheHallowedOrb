#ifndef SPRITE_H
#define SPRITE_H

#include "toolbox.h"
#include "ResourceManager.h"

class Sprite
{
    public:
        Sprite(sf::Texture& texture, int width, int height, int frameCount, int speed, float offsetX, float offsetY, bool loop);

        void step();
        void draw(sf::RenderWindow& window, sf::RenderStates states = sf::RenderStates::Default);

        void randomizeFrame();
        void setFlipped(bool f);
        void setFrozen(bool f);
        void setOrigin(float x, float y);
        void setPosition(float x, float y);
        void setRotation(float theta);
        void setFrame(int f);
        void setScale(float sx, float sy);
        void setScale(float s);
        void setColor(sf::Color color);
        float percentComplete();
        int getFrame();
        int getTotalFrames();
        int getTime();
        void setSpeed(int s);
        sf::Vector2i getSize();
        void reset();
        bool isFinished();

    private:
        sf::Sprite sprite;
        int w;
        int h;
        int frame;
        int numFrames;
        int timer;
        int speed;
        float angle;
        float scaleX;
        float scaleY;
        sf::Vector2f offset;
        bool flipped;
        bool reverse;
        bool frozen;
        bool loop;
        bool done;
        int rows;
        int cols;


};

#endif // SPRITE_H
