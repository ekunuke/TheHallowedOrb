#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Sprite.h"

class Player : public Entity
{
    public:
        static const float WIDTH;
        static const float HEIGHT;
        static const float ACCEL_SPEED;
        static const float JUMP_SPEED;
        static const float JUMP_ACCEL_SPEED;
        static const int JUMP_ACCEL_FRAMES;
        static const float MOVE_SPEED;
        static const float MAX_ARROW_SPEED;
        static const int MAX_POWER; /// in number of frames
        static const int STARTING_HEALTH;
        static const int SHOW_HEALTH_TIME;
        static const int INVINCIBLE_FRAMES;
        Player(Level& level, float x, float y);
        virtual ~Player();


        virtual void step();
        virtual void draw(sf::RenderWindow& window);
        virtual void collideEntity(Entity *other);
        void moveLeft();
        void moveRight();
        void jump();
        void startShoot();
        void stopShoot();
        void tryMuteMusic();

    private:
        void hurt();
        void drawPowerIndicator(sf::RenderWindow& window);
        void drawHealth(sf::RenderWindow& window);
        int power;
        bool charging;
    public:
        bool hasArrow;
    private:
        bool firstTimeFalling;
        int stunTimer;
        bool facingRight;
        bool prevJumpHeld;
        bool jumpHeld;
        bool jumpAccelerating;
        bool touchingPedestal;
        int muteTimer;
        int muteTextFade;

        int jumpFrame;

    public:
        bool dead;

    public:
        int invincibleTimer;
        int showHealthTimer;
        int maxHealth;
        int health;
    private:
        Sprite muteSprite;
        Sprite heartSprite;
        Sprite ballSprite;
        Sprite standSprite;
        Sprite walkSprite;
        Sprite airSprite;
        Sprite deadSprite;
        Sprite* currSprite;
};

#endif // PLAYER_H
