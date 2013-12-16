#include "Sprite.h"

Sprite::Sprite(sf::Texture& texture, int width, int height, int frameCount, int speed, float offsetX, float offsetY, bool loop):
    sprite(texture),
    w(width),
    h(height),
    frame(0),
    numFrames(frameCount),
    timer(0),
    speed(speed),
    angle(0),
    scaleX(1.0),
    scaleY(1.0),
    offset(offsetX, offsetY),
    flipped(false),
    reverse(false),
    frozen(false),
    loop(loop),
    done(false)
{
    sprite.setOrigin(w/2, h/2);
    sprite.setTextureRect(sf::IntRect(0, 0, w, h));
    if(w > (int)(texture.getSize().x))
    {
        w = texture.getSize().x;
    }
    if( h > (int)(texture.getSize().y))
    {
        h = texture.getSize().y;
    }
    rows = texture.getSize().y / h;
    cols = texture.getSize().x / w;
    if(numFrames == -1)
    {
        numFrames = rows * cols;
    }
}

void Sprite::step()
{
    if(frozen)
    {
        return;
    }
    timer++;
    if(timer > speed)
    {
        if(reverse)
        {
            frame--;
        }
        else
        {
            frame++;
        }
        if(frame < 0)
        {
            if(loop)
            {
                frame = numFrames - 1;
            }
            else
            {
                frame = 0;
                done = true;
            }
        }
        if(frame >= numFrames)
        {
            if(loop)
            {
                frame = 0;
            }
            else
            {
                frame = numFrames-1;
                done = true;
            }
        }
//        sprite.setTextureRect( sf::IntRect(col*texture.spriteWidth, row*texture.spriteHeight,
//                                       texture.spriteWidth, texture.spriteHeight));
        int x = (frame % cols) * w;
        int y = (int)(frame / cols) * h;
        sprite.setTextureRect(sf::IntRect(x, y, w, h));
        timer = 0;
    }
}
void Sprite::draw(sf::RenderWindow& window, sf::RenderStates states)
{
    if(flipped)
    {
        sprite.setScale(-1 * scaleX, 1 * scaleY);
    }
    else
    {
        sprite.setScale(1 * scaleX, 1 * scaleY);
    }

    window.draw(sprite, states);
}

void Sprite::randomizeFrame()
{
    frame = rand()%numFrames;
    timer = rand()%speed;
}
void Sprite::setFlipped(bool f)
{
    flipped = f;
}
void Sprite::setFrozen(bool f)
{
    frozen = f;
}

void Sprite::setOrigin(float x, float y)
{
    sprite.setOrigin(x, y);
}

void Sprite::setPosition(float x, float y)
{
    if(flipped)
        sprite.setPosition((int)(x + offset.x), (int)(y-offset.y));
    else
        sprite.setPosition((int)(x - offset.x), (int)(y-offset.y));
}
void Sprite::setRotation(float theta)
{
    angle = theta;
    sprite.setRotation(theta);
}
void Sprite::setFrame(int f)
{
    frame = f;
    timer = 0;
    int x = (frame % cols) * w;
    int y = (int)(frame / cols) * h;
    sprite.setTextureRect(sf::IntRect(x, y, w, h));
}
void Sprite::setScale(float sx, float sy)
{
    scaleX = sx;
    scaleY = sy;
}

void Sprite::setScale(float s)
{
    setScale(s, s);
}

void Sprite::setColor(sf::Color color)
{
    sprite.setColor(color);
}
float Sprite::percentComplete()
{
    return (frame * speed + timer) / (float)(numFrames * speed);
}
int Sprite::getFrame()
{
    return frame;
}
int Sprite::getTotalFrames()
{
    return numFrames * speed;
}
int Sprite::getTime()
{
    return timer;
}
void Sprite::setSpeed(int s)
{
    speed = s;
}
sf::Vector2i Sprite::getSize()
{
    return sf::Vector2i(w, h);
}
void Sprite::reset()
{
    frame = 0;
    timer = 0;
    done = false;
    sprite.setTextureRect(sf::IntRect(0, 0, w, h));
}
bool Sprite::isFinished()
{
    return done;
}
