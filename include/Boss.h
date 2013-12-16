#ifndef BOSS_H
#define BOSS_H

#include "Entity.h"
#include "Sprite.h"

class Boss : public Entity
{
    public:
        static const int ANGRY_HEALTH_THRESHOLD;
        static const int HEALTH;
        static const int SUMMON_TIME;
        static const int SPAWN_TIME;
        static const int INVUL_TIME;
        static const float WIDTH;
        static const float HEIGHT;
        static const float ACCEL_RATE;
        static const float MAX_SPEED;
        static const float CHARGE_SPEED;
        static const float CHARGE_DISTANCE;
        Boss(Level& level, float x, float y);
        virtual ~Boss();

        virtual void step();
        virtual void draw(sf::RenderWindow& window);
        virtual void collideEntity(Entity* other);

        bool hurt();

    private:
        void drawHealthBar(sf::RenderWindow& window);
        enum BossActionType
        {
            ACTION_IDLE,
            ACTION_MOVE_LEFT_PLAYER,
            ACTION_MOVE_RIGHT_PLAYER,
            ACTION_MOVE_ABOVE_PLAYER,
            ACTION_MOVE_BELOW_PLAYER,
            ACTION_SET_ANGLE_TO_PLAYER,
            ACTION_CHARGE,
            ACTION_SUMMON
        };

        struct BossAction
        {
            BossActionType type;
            int timer;
            BossAction(BossActionType type, int timer):
                type(type),
                timer(timer)
                {
                }
        };
        bool moveSideOfPlayer(int dir);

        void setAngleToPlayer();
        bool chargePlayer();
        bool summon();
        void addAction(BossActionType type, int time = 0);
        void addNewActions();

        bool facingRight;

        bool angry;

        int spawnTimer;

        int summonTimer;
        bool summoning;

        int invulTimer;

        int health;
        float drawHealth;
        float angle;
        bool charging;
        sf::Vector2f originalPosition;
        Sprite sprite;
        queue<BossAction> actionQueue;
};

#endif // BOSS_H
