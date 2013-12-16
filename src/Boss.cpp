#include "Boss.h"
#include "Player.h"
#include "Particle.h"
#include "Suicider.h"

const int Boss::ANGRY_HEALTH_THRESHOLD = 3;
const int Boss::HEALTH = 8;
const int Boss::SUMMON_TIME = 60; // 1 second channel
const int Boss::SPAWN_TIME = 90;
const int Boss::INVUL_TIME = 40;
const float Boss::WIDTH = 20;
const float Boss::HEIGHT = 32;

const float Boss::ACCEL_RATE = 0.1;
const float Boss::MAX_SPEED = 1.4;

const float Boss::CHARGE_SPEED = 4;
const float Boss::CHARGE_DISTANCE = 150;

Boss::Boss(Level& level, float x, float y):
    Entity(level, TYPE_BOSS, x, y, WIDTH, HEIGHT),
    facingRight(false),
    angry(false),
    spawnTimer(SPAWN_TIME),
    summonTimer(0),
    summoning(false),
    invulTimer(0),
    health(HEALTH),
    drawHealth(0),
    angle(0),
    charging(false),
    sprite(ResourceManager::getTexture("boss.png"), 32, 32, -1, 10, 6, 0, true)
{
    ResourceManager::playSound("boss_spawned.wav");
    addAction(ACTION_IDLE, 120);
    enemy = true;
    for(unsigned i = 0; i < 400; i++)
    {
        level.addEntity(new Particle(level, x + rand()%(int)(w), y + rand()%(int)(h), Particle::PARTICLE_FIRE));
    }
}

Boss::~Boss()
{
    //dtor
}

void Boss::step()
{
    if(spawnTimer > 0)
    {
        spawnTimer --;
    }
    if(invulTimer > 0)
    {
        invulTimer--;
    }
    if(actionQueue.empty())
    {
        addNewActions();
    }
    BossActionType actionType = actionQueue.front().type;
    switch(actionType)
    {
    case ACTION_IDLE:
        actionQueue.front().timer--;
        dx *= 0.9;
        dy *= 0.9;
        if(actionQueue.front().timer <= 0)
        {
            actionQueue.pop();
        }
        break;
    case ACTION_MOVE_LEFT_PLAYER:
        if(moveSideOfPlayer(0))
        {
            actionQueue.pop();
        }
        break;
    case ACTION_MOVE_RIGHT_PLAYER:
        if(moveSideOfPlayer(1))
        {
            actionQueue.pop();
        }
        break;
    case ACTION_MOVE_ABOVE_PLAYER:
        if(moveSideOfPlayer(2))
        {
            actionQueue.pop();
        }
        break;
    case ACTION_MOVE_BELOW_PLAYER:
        if(moveSideOfPlayer(3))
        {
            actionQueue.pop();
        }
        break;
    case ACTION_SUMMON:
        if(summon())
        {
            actionQueue.pop();
        }
        break;
    case ACTION_SET_ANGLE_TO_PLAYER:
        setAngleToPlayer();
        actionQueue.pop();
        break;
    case ACTION_CHARGE:
        if(chargePlayer())
        {
            actionQueue.pop();
        }
        break;
    }

    if(dx > 0)
    {
        facingRight = true;
    }
    else
    {
        facingRight = false;
    }

    x += dx;
    y += dy;

    sprite.step();
}

void Boss::draw(sf::RenderWindow& window)
{
    drawHealthBar(window);
    if(invulTimer > 0)
    {
        if(invulTimer % 4 < 2)
        {
            return;
        }
    }
    if(charging)
    {
        sf::Vector2f afterImageOffset({cosf(angle) * -5, sinf(angle) * -5});
        for(unsigned i = 0; i < 2; i++)
        {
            sprite.setPosition(x + w/2 + afterImageOffset.x * (i+1), y + h/2 + afterImageOffset.y * (i+1));
            sprite.setColor(sf::Color(255, 255, 255, 150));
            sprite.draw(window);
        }
    }
    if(angry)
    {
        sprite.setColor(sf::Color(255, 225, 225));
    }
    else
    {
        sprite.setColor(sf::Color(255, 255, 255));
    }


    sprite.setFlipped(!facingRight);
    sprite.setPosition(x + w/2, y + h/2);
    if(summoning)
    {
        float percentSummon = summonTimer / (float)SUMMON_TIME;
        sprite.setPosition(x + w/2 + rand()%4 - 2, y + h/2 + rand()%4 - 2);
        sprite.setColor(sf::Color(255, 255 * percentSummon, 255 * percentSummon));
    }
    if(spawnTimer > 0)
    {
        sprite.setColor(sf::Color(255, 255, 255, (1 - (spawnTimer / (float)SPAWN_TIME)) * 255));
    }
    sprite.draw(window);
}

void Boss::drawHealthBar(sf::RenderWindow& window)
{
    if(drawHealth > health)
    {
        drawHealth += (health - drawHealth) / 10.0f;
        if(fabs(drawHealth - health) < (0.06))
        {
            drawHealth = health;
        }
    }
    else if(drawHealth < health)
    {
        drawHealth += 0.06;
        if(drawHealth > health)
        {
            drawHealth = health;
        }
    }

    sf::RectangleShape barBg({200, 6});

    barBg.setFillColor(sf::Color::Black);
    barBg.setPosition (100, 20);
    window.draw(barBg);

    sf::RectangleShape barFg({196 * (drawHealth / (float)HEALTH), 2});
    if(invulTimer > 0)
    {
        if(invulTimer % 6  < 3)
        {
            return;
        }
    }
    barFg.setFillColor(sf::Color(208, 70, 72));
    barFg.setPosition(barBg.getPosition() + sf::Vector2f(2, 2));
    window.draw(barFg);

    sf::RectangleShape notch({2,6});
    notch.setFillColor(sf::Color::Black);
    for(int i = 1; i < HEALTH; i++)
    {
        notch.setPosition(barBg.getPosition() + sf::Vector2f(2 + i * (200 / HEALTH), 0));
        window.draw(notch);
    }
}

void Boss::collideEntity(Entity* other)
{

}

bool Boss::hurt()
{
    if(spawnTimer > 0)
    {
        return false;
    }
    if(invulTimer > 0)
    {
        return false;
    }
    if(charging)
    {
        return false;
    }
    invulTimer = INVUL_TIME;
    health--;
    if(health <= 0)
    {
        die = true;

        for(unsigned i = 0; i < 60; i++)
        {
            level.addEntity(new Particle(level, x + rand()%(int)(w), y + rand()%(int)(h), Particle::PARTICLE_BLOOD));
        }
    }
    else if(health <= ANGRY_HEALTH_THRESHOLD)
    {
        angry = true;
    }
    return true;
}

bool Boss::moveSideOfPlayer(int dir)
{
    static float GOOD_ENOUGH_DISTANCE = 32;

    Player* player = level.getPlayer();
    sf::Vector2f offset;
    offset.x = ((dir == 0) ? 100 : (dir == 1 ? -100 : 0));
    offset.y = ((dir == 2) ? 100 : (dir == 3 ? -100 : 0));
    float distX = player->x + player->w/2 + (offset.x) - (x + w/2);
    float distY = player->y + player->h/2 + (offset.y) - (y + h/2);

    float magnitude = sqrtf(distX * distX + distY * distY);
    if(magnitude <= GOOD_ENOUGH_DISTANCE)
    {
        dx = 0;
        dy = 0;
        return true;
    }
    dx += ACCEL_RATE * (distX / magnitude);
    dy += ACCEL_RATE * (distY / magnitude);
    magnitude = sqrtf(dx * dx + dy * dy);
    if(magnitude > MAX_SPEED)
    {
        dx *= MAX_SPEED / magnitude;
        dy *= MAX_SPEED / magnitude;
    }

    return false;
}

void Boss::setAngleToPlayer()
{
    ResourceManager::playSound("charge.wav");
    originalPosition.x = x;
    originalPosition.y = y;
    Player* player = level.getPlayer();
    angle = atan2f(player->y + player->h/2 - (y + h/2),
                   player->x + player->w/2 - (x + w/2));
    facingRight = player->x + player->w/2 > x + w/2;
}

bool Boss::chargePlayer()
{
    float distanceFromOrigin = sqrtf(powf(originalPosition.x - x, 2) +
                                      powf(originalPosition.y - y, 2));
    if(distanceFromOrigin >= CHARGE_DISTANCE)
    {
        charging = false;
        dx = 0;
        dy = 0;
        return true;
    }
    charging = true;
    dx = cosf(angle) * CHARGE_SPEED;
    dy = sinf(angle) * CHARGE_SPEED;
    return false;
}

bool Boss::summon()
{
    summoning = true;
    if(summonTimer > 0)
    {
        summonTimer--;

        return false;
    }
    ResourceManager::playSound("boss_summon.wav");
    summoning = false;
    level.addEntity(new Suicider(level, x - 16, y));
    level.addEntity(new Suicider(level, x + w, y));
    return true;
}

void Boss::addAction(BossActionType type, int time)
{
    actionQueue.push(BossAction(type, time));
}

void Boss::addNewActions()
{
    summonTimer = SUMMON_TIME;
    ///half chance to charge
//    if(rand()%2 == 0)
    if(!angry)
    {
        addAction(ACTION_IDLE, 30);

        int dir = rand()%4;
        if(dir == 0)
        {
            addAction(ACTION_MOVE_LEFT_PLAYER);
        }
        else if(dir == 1)
        {
            addAction(ACTION_MOVE_RIGHT_PLAYER);
        }
        else if(dir == 2)
        {
            addAction(ACTION_MOVE_ABOVE_PLAYER);
        }
        else
        {
            addAction(ACTION_MOVE_BELOW_PLAYER);
        }

        addAction(ACTION_IDLE, 30);
        addAction(ACTION_SET_ANGLE_TO_PLAYER);
        addAction(ACTION_CHARGE);
    }
    else
    {
        if(rand()%3 == 0)
        {
            addAction(ACTION_IDLE, 60);
            addAction(ACTION_SUMMON);
            addAction(ACTION_IDLE, 60);
        }
        else
        {

            if(rand()%2 == 0)
            {
                addAction(ACTION_MOVE_BELOW_PLAYER);
                addAction(ACTION_IDLE, 40);
                addAction(ACTION_SET_ANGLE_TO_PLAYER);
                addAction(ACTION_CHARGE);
                addAction(ACTION_IDLE, 20);
                addAction(ACTION_MOVE_ABOVE_PLAYER);
                addAction(ACTION_IDLE, 40);
                addAction(ACTION_SET_ANGLE_TO_PLAYER);
                addAction(ACTION_CHARGE);
            }
            else
            {
                addAction(ACTION_MOVE_LEFT_PLAYER);
                addAction(ACTION_IDLE, 40);
                addAction(ACTION_SET_ANGLE_TO_PLAYER);
                addAction(ACTION_CHARGE);
                addAction(ACTION_IDLE, 20);
                addAction(ACTION_MOVE_RIGHT_PLAYER);
                addAction(ACTION_IDLE, 40);
                addAction(ACTION_SET_ANGLE_TO_PLAYER);
                addAction(ACTION_CHARGE);
            }
            addAction(ACTION_IDLE, 20);
        }
    }
}
