#include "Player.h"
#include "Projectile.h"
#include "Particle.h"

const float Player::WIDTH = 6;
const float Player::HEIGHT = 13;
const float Player::ACCEL_SPEED = 0.6;
const float Player::MOVE_SPEED = 1.8;
const float Player::JUMP_SPEED = 2;
const float Player::JUMP_ACCEL_SPEED = 0.16;
const int Player::JUMP_ACCEL_FRAMES = 12;
const float Player::MAX_ARROW_SPEED = 6;
const int Player::MAX_POWER = 45;
const int Player::STARTING_HEALTH = 5;
const int Player::SHOW_HEALTH_TIME = 120;
const int Player::INVINCIBLE_FRAMES = 90;

Player::Player(Level& level, float x, float y):
    Entity(level, TYPE_PLAYER, x, y, WIDTH, HEIGHT),
    power(0),
    charging(false),
    hasArrow(false),
    firstTimeFalling(true),
    stunTimer(0),
    facingRight(true),
    prevJumpHeld(false),
    jumpHeld(false),
    jumpAccelerating(false),
    touchingPedestal(false),
    muteTimer(0),
    muteTextFade(0),
    jumpFrame(0),
    dead(false),
    invincibleTimer(0),
    showHealthTimer(0),
    maxHealth(STARTING_HEALTH),
    health(STARTING_HEALTH),
    muteSprite(ResourceManager::getTexture("mute.png"), 16, 16, -1, 0, 0, 0, false),
    heartSprite(ResourceManager::getTexture("heart.png"), 5, 5, -1, 0, 0, 0, false),
    ballSprite(ResourceManager::getTexture("projectile.png"), 6, 6, -1, 0, 0, 0, false),
    standSprite(ResourceManager::getTexture("player_stand.png"), 16, 16, -1, 0, 0, 1, true),
    walkSprite(ResourceManager::getTexture("player_walk.png"), 16, 16, -1, 4, 0, 1, true),
    airSprite(ResourceManager::getTexture("player_jumpfall.png"), 16, 16, -1, 0, 0, 1, false),
    deadSprite(ResourceManager::getTexture("player_dead.png"), 16, 16, -1, 0, 0, 1, false),
    currSprite(&standSprite)
{
    ballSprite.setOrigin(0, 0);
}

Player::~Player()
{
    //dtor
}

void Player::step()
{
    if(hasArrow && charging && power < MAX_POWER)
    {
        power++;
    }
    if(dx > 0)
    {
        facingRight = true;
    }
    else if(dx < 0)
    {
        facingRight = false;
    }
//    if(dy < 0 && !jumpHeld)
//    {
//        dy /= (1.5f);
//    }

    if(!jumpHeld)
    {
        jumpAccelerating = false;
    }
    if(jumpHeld && jumpAccelerating)
    {
        if(jumpFrame < JUMP_ACCEL_FRAMES)
        {
            dy -= JUMP_ACCEL_SPEED;
            jumpFrame++;
        }
    }
    prevJumpHeld = jumpHeld;
    jumpHeld = false;

    if(stunTimer > 0)
    {
        stunTimer--;
        dx = 0;
        dy = 0;
    }
    dy += GRAVITY;
    x += dx;
    y += dy;
    if(landedDown)
    {
        if(firstTimeFalling)
        {
            firstTimeFalling = false;
            stunTimer = 30;
            level.shake(10);
            showHealthTimer = 60;
            ResourceManager::playSound("landing.wav");
        }
        dx *= 0.8f;
    }
    else
    {
        dx *= 0.9;
    }

    if(invincibleTimer > 0)
    {
        invincibleTimer --;
    }
    if(showHealthTimer > 0)
    {
        showHealthTimer--;
    }
    if(muteTimer > 0)
    {
        muteTimer --;
    }
    touchingPedestal = false;
    collideLevel();
    currSprite->step();
}

void Player::draw(sf::RenderWindow& window)
{
    if(showHealthTimer > 0)
    {
        drawHealth(window);
    }
    if(invincibleTimer > 0 && invincibleTimer % 10 < 5)
    {
    }
    else
    {
        if(!dead && hasArrow && stunTimer == 0)
        {
            ballSprite.setPosition(x + (facingRight ? -2 : 2), y + 2);
            ballSprite.draw(window);
        }
        if(stunTimer > 0)
        {
            currSprite = &deadSprite;
            currSprite->setFrame(1);
        }
        else if(dead)
        {
            currSprite = &deadSprite;
            if(landedDown)
            {
                currSprite->setFrame(1);
            }
            else
            {
                currSprite->setFrame(0);
            }
        }
        else if(!landedDown)
        {
            currSprite = &airSprite;
            if(dy < 0)
            {
                currSprite->setFrame(0);
            }
            else
            {
                currSprite->setFrame(1);
            }
        }
        else
        {
            if(fabs(dx) > 0.1)
            {
                currSprite = &walkSprite;
            }
            else
            {
                currSprite = &standSprite;
            }
        }
        currSprite->setFlipped(!facingRight);
        currSprite->setPosition(x + w/2, y + h/2);
        currSprite->draw(window);
    }
    if(!dead && touchingPedestal && fabs(dx) < 0.1)
    {
        if(muteTimer > 0)
        {
            muteSprite.setFrame(ResourceManager::isMuted() ? 0 : 1);
            muteSprite.setPosition(30, 30);
            muteSprite.setColor(sf::Color(255, 255, 255, 255 * (muteTimer / 60.f)));
            muteSprite.draw(window);
        }
        else
        {
            if(muteTextFade < 40)
            {
                muteTextFade += 1;
            }
            sf::Text muteText("Press down over the pedestal to mute music",
                              ResourceManager::getDefaultFont(),
                              8);
            muteText.setColor(sf::Color(0, 0, 0, muteTextFade));
            muteText.setPosition(19, 16);
            window.draw(muteText);

            ///draw message?
        }
    }
    else
    {
        if(muteTextFade > 0)
        {
            muteTextFade--;
        }
    }
    if(charging)
    {
        drawPowerIndicator(window);
    }
}

void Player::collideEntity(Entity* other)
{
    if(other->entityType == TYPE_PROJECTILE)
    {
        Projectile* proj = static_cast<Projectile*>(other);
        if(!proj->ragnarok && (proj->stuck || proj->onPedestal))
        {
            proj->die = true;
            hasArrow = true;
            for(unsigned i = 0; i < 10; i++)
            {
                level.addEntity(new Particle(level, x + rand()%(int)(w), y + rand()%(int)(h), Particle::PARTICLE_BLUE));
                level.addEntity(new Particle(level, x + rand()%(int)(w), y + rand()%(int)(h), Particle::PARTICLE_DEEP_BLUE));
            }
        }
    }
    else if(other->enemy)
    {
        hurt();
    }
    else if(other->entityType == TYPE_POWERUP)
    {
        ResourceManager::playSound("powerup.wav");
        health++;
        other->die = true;
        for(unsigned i = 0; i < 10; i++)
        {
            level.addEntity(new Particle(level,
                                         x + rand()%(int)w,
                                         y + rand()%(int)h, Particle::PARTICLE_HEALTH));
        }
    }
    else if(other->entityType == TYPE_PEDESTAL)
    {
        touchingPedestal = true;
    }
}

void Player::moveLeft()
{
    dx -= ACCEL_SPEED;
    if(dx < -MOVE_SPEED)
    {
        dx = -MOVE_SPEED;
    }
}

void Player::moveRight()
{
    dx += ACCEL_SPEED;
    if(dx > MOVE_SPEED)
    {
        dx = MOVE_SPEED;
    }
}

void Player::jump()
{
    if(!prevJumpHeld && landedDown)
    {
        dy = -JUMP_SPEED;
        jumpFrame = 0;
        jumpAccelerating = true;
        ResourceManager::playSound("bouncy.wav");
    }
    jumpHeld = true;
}

void Player::startShoot()
{
    charging = true;

}

void Player::stopShoot()
{
    if(hasArrow)
    {
        float xSpeed = MAX_ARROW_SPEED * power / (float)MAX_POWER;
        float yVel = -0.85 * power / (float)MAX_POWER;
        hasArrow = false;
        level.addEntity(new Projectile(level, x + (facingRight ? 3 : -3), y, xSpeed * (facingRight ? 1 : -1), yVel));
        power = 0;
        ResourceManager::playSound("player_shoot.wav");
    }
    charging = false;
}

void Player::tryMuteMusic()
{
    if(touchingPedestal && fabs(dx) < 0.1)
    {
        muteTimer = 60;
        ResourceManager::toggleMute();
    }
}

void Player::hurt()
{
    if(invincibleTimer > 0)
    {
        return;
    }
    showHealthTimer = SHOW_HEALTH_TIME;
    health--;
    dx += facingRight?-1 : 1;
    dy -= 2;

    if(health <= 0)
    {
        dead = true;
        palpable = false;

        ResourceManager::playSound("player_death.wav");
    }
    else
    {
        ResourceManager::playSound("player_hurt.wav");
    }
    invincibleTimer = INVINCIBLE_FRAMES;
}
void Player::drawPowerIndicator(sf::RenderWindow& window)
{
    if(dead || !hasArrow)
    {
        return;
    }
    sf::RectangleShape backBox({4, 4});
            backBox.setFillColor(sf::Color(50, 50, 50));
    sf::RectangleShape tickBox({2, 2});
            tickBox.setFillColor(sf::Color(109, 194, 202));

    for(int i = 0; i < MAX_POWER; i+=10)
    {
        backBox.setPosition(x - 7 + 3 * (i/10), y - 6);
        window.draw(backBox);
        if(i < power)
        {
            tickBox.setPosition(x - 6 + 3 * (i/10), y - 5);
            window.draw(tickBox);
        }
    }
}

void Player::drawHealth(sf::RenderWindow& window)
{
    if(dead)
    {
        return;
    }
    int numBoxes = health;
    bool blinkLastBox = false;
    if(invincibleTimer > 0 && showHealthTimer > SHOW_HEALTH_TIME / 1.5)
    {
        numBoxes++;
        blinkLastBox = true;
    }

    for(int i = 0; i < numBoxes; i++)
    {
        if(blinkLastBox && i == numBoxes-1 && showHealthTimer % 5 < 2)
        {
            break;
        }
        heartSprite.setPosition(x + w/2 + i * 6 +2 - (numBoxes / 2.f * 6.f), y - 10);
        heartSprite.draw(window);

    }
}
