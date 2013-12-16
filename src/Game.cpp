#include "Game.h"
#include "Entity.h"
#include "Player.h"
#include "Zombie.h"
#include "Lunger.h"
#include "Flyer.h"
#include "Suicider.h"
#include "Boss.h"
#include "Powerup.h"
#include "Projectile.h"

Game::Game():
    frame(0),
    time(0),
    downSprite(ResourceManager::getTexture("down.png"), 20, 20, -1, 0, 0, 0, false),
    leftSprite(ResourceManager::getTexture("left.png"), 20, 20, -1, 0, 0, 0, false),
    rightSprite(ResourceManager::getTexture("right.png"), 20, 20, -1, 0, 0, 0, false),
    throwSprite(ResourceManager::getTexture("throw.png"), 20, 20, -1, 0, 0, 0, false),
    jumpSprite(ResourceManager::getTexture("jump.png"), 20, 20, -1, 0, 0, 0, false),

    titleSprite(ResourceManager::getTexture("title.png"), 200, 100, -1, 0, 0, 0, false),
    gamepadSprite(ResourceManager::getTexture("gamepad.png"), 98, 20, -1, 0, 0, 0, false),
    keyboardSprite(ResourceManager::getTexture("keyboard.png"), 98, 20, -1, 0, 0, 0, false),
    playerFallSprite(NULL),
    view({12.5f * Tile::TILE_SIZE, 9.5f * Tile::TILE_SIZE}, {400, 300}),
    level(25, 19),
    player(NULL),
    currWave(0),
    allWaveEnemiesSpawned(false),
    endingPlayerY(300),
    endCinematic(false),
    finalCutscene(false),
    escaped(false),

    configuring(true),
    configureExitTimer(0),
    gameStarted(false),
    slowMotion(false),
    prevEscape(false),
    escape(false),
    paused(false),
    RAGNAROK(false)
{
    downSprite.setOrigin(0, 0);
    leftSprite.setOrigin(0, 0);
    rightSprite.setOrigin(0, 0);
    throwSprite.setOrigin(0, 0);
    jumpSprite.setOrigin(0, 0);

    gamepadSprite.setOrigin(0, 0);
    keyboardSprite.setOrigin(0, 0);
    titleSprite.setOrigin(0, 0);
    overlayTexture.create(400, 300);
    keys.resize(sf::Keyboard::KeyCount, false);
    prevKeys.resize(sf::Keyboard::KeyCount, false);
    // ResourceManager::toggleMute(); /// Just for devving

    for(unsigned i = 0; i < 40; i++)
    {
        fallingParticles.push_back({(float)(9 * Tile::TILE_SIZE + rand()%(Tile::TILE_SIZE + 2)), (float)(rand()%300)});
    }
    newGame();
}

Game::~Game()
{
    for(unsigned i = 0; i < entities.size(); i++)
    {
        delete entities[i];
    }
    if(playerFallSprite)
    {
        delete playerFallSprite;
    }
}

void Game::newGame()
{
    ResourceManager::stopSong("static.ogg");
    deathAnimTimer = DEATH_ANIM_TIME;
    gameStarted = false;
    titleAlpha = 255;
    for(unsigned i = 0; i < entities.size(); i++)
    {
        delete entities[i];
    }
    entities.clear();
    player = new Player(level, 150, -50);
    player->dy = 3;
    level.setEntities(&entities);
    level.buildLevel();
    level.setPlayer(player);
    level.addEntity(player);
    level.openTop(false);
    spawnLocations.clear();
    allWaveEnemiesSpawned = false;

    currWave = 0;

    waveCoolDownTimer = WAVE_COOLDOWN_TIME;
    waveTimer = 0;

//    for(unsigned i = 0; i < 4; i++)
//    {
//        level.addEntity(new Lunger(level, rand()%((int)(1 + (level.w-2) * Tile::TILE_SIZE)),
//                                          rand()%((int)(1 + (level.h-2) * Tile::TILE_SIZE))));
//        level.addEntity(new Zombie(level, rand()%((int)(1 + (level.w-2) * Tile::TILE_SIZE)),
//                                          rand()%((int)(1 + (level.h-2) * Tile::TILE_SIZE))));
//    }

}


void Game::nextWave()
{
    spawnLocations.clear();
    allWaveEnemiesSpawned = false;
    waveCoolDownTimer = WAVE_COOLDOWN_TIME;
    waveTimer = 0;
    currWave++;
}

void Game::doWaveLogicA()
{
    static bool left = false;
    if(waveTimer == 0)
    {
        left = false;
        time = 0;
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
    }

    if(waveTimer % (140) == 0)
    {
        if(left)
        {
            Zombie* zombie = new Zombie(level, (1 * Tile::TILE_SIZE),
                                              (17 * Tile::TILE_SIZE));
            level.addEntity(zombie);
        }
        else
        {
            Zombie* zombie = new Zombie(level, (23 * Tile::TILE_SIZE),
                                              (17 * Tile::TILE_SIZE));

            level.addEntity(zombie);
        }
        left = !left;
    }

    if(waveTimer == 60 * 5)
    {
        level.addEntity(new Powerup(level, 12.5 * Tile::TILE_SIZE - 3, 10 * Tile::TILE_SIZE));
    }
    if(waveTimer > 60 * 15)
    {
        allWaveEnemiesSpawned = true;
    }
}

void Game::doWaveLogicB()
{
    static bool left = false;
    if(waveTimer == 0)
    {
        left = false;
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 1 * Tile::TILE_SIZE});
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 1 * Tile::TILE_SIZE});
    }
    if(waveTimer % (360) == 0)
    {
        level.addEntity(new Zombie(level, (1 * Tile::TILE_SIZE),
                                          (17 * Tile::TILE_SIZE)));
        level.addEntity(new Zombie(level, (23 * Tile::TILE_SIZE),
                                          (17 * Tile::TILE_SIZE)));
        if(left)
        {
            level.addEntity(new Flyer(level, (23 * Tile::TILE_SIZE),
                                              (1 * Tile::TILE_SIZE)));
        }
        else if(!left)
        {
            level.addEntity(new Flyer(level, (1 * Tile::TILE_SIZE),
                                              (1 * Tile::TILE_SIZE)));
        }
        left = !left;
    }

    if(waveTimer == 60 * 5)
    {
        level.addEntity(new Powerup(level, 12.5 * Tile::TILE_SIZE - 3, 10 * Tile::TILE_SIZE));
    }
    if(waveTimer > 60 * 18)
    {
        allWaveEnemiesSpawned = true;
    }
}

void Game::doWaveLogicC()
{
    if(waveTimer == 0)
    {
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
        spawnLocations.push_back({12 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
    }
    if(waveTimer % (120) == 0)
    {
        level.addEntity(new Zombie(level, (1 * Tile::TILE_SIZE),
                                          (17 * Tile::TILE_SIZE)));
        level.addEntity(new Zombie(level, (23 * Tile::TILE_SIZE),
                                          (17 * Tile::TILE_SIZE)));
    }
    if(waveTimer%(180) == 0)
    {
        level.addEntity(new Lunger(level, (12 * Tile::TILE_SIZE),
                                          (17 * Tile::TILE_SIZE)));
    }
    if(waveTimer == 60 * 5)
    {
        level.addEntity(new Powerup(level, 12.5 * Tile::TILE_SIZE - 3, 10 * Tile::TILE_SIZE));
    }
    if(waveTimer > 60 * 15)
    {
        allWaveEnemiesSpawned = true;
    }
}

void Game::doWaveLogicD()
{
    static bool left = false;
    if(waveTimer == 0)
    {
        left = true;
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 1 * Tile::TILE_SIZE});
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 1 * Tile::TILE_SIZE});
    }
    if(waveTimer % (180) == 0)
    {
        Zombie* zombie = new Zombie(level, (1 * Tile::TILE_SIZE),
                                          (17 * Tile::TILE_SIZE));

        level.addEntity(zombie);
        zombie = new Zombie(level, (23 * Tile::TILE_SIZE),
                                          (17 * Tile::TILE_SIZE));
        level.addEntity(zombie);
    }

    if(waveTimer == 60 * 5)
    {
        level.addEntity(new Powerup(level, 12.5 * Tile::TILE_SIZE - 3, 10 * Tile::TILE_SIZE));
    }
    if(waveTimer % 240 == 0)
    {
        if(left)
        {
            level.addEntity(new Suicider(level, (1 * Tile::TILE_SIZE),
                                              (1 * Tile::TILE_SIZE)));
        }
        else
        {
            level.addEntity(new Suicider(level, (23 * Tile::TILE_SIZE),
                                              (1 * Tile::TILE_SIZE)));
        }
        left = !left;
    }
    if(waveTimer > 60 * 15)
    {
        allWaveEnemiesSpawned = true;
    }
}
void Game::doWaveLogicE()
{
    static bool left = false;
    if(waveTimer == 0)
    {
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 1 * Tile::TILE_SIZE});
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 1 * Tile::TILE_SIZE});
        left = true;
    }
    if(waveTimer % (240) == 0)
    {
        Flyer* flyer = new Flyer(level, (1 * Tile::TILE_SIZE),
                                          (17 * Tile::TILE_SIZE));

        level.addEntity(flyer);
        flyer = new Flyer(level, (23 * Tile::TILE_SIZE),
                                          (17 * Tile::TILE_SIZE));
        level.addEntity(flyer);
    }
    if(waveTimer % 300 == 0)
    {
        if(left)
        {
            level.addEntity(new Suicider(level, (1 * Tile::TILE_SIZE),
                                              (1 * Tile::TILE_SIZE)));
            level.addEntity(new Flyer(level, (23 * Tile::TILE_SIZE),
                                              (1 * Tile::TILE_SIZE)));
        }
        else
        {
            level.addEntity(new Suicider(level, (23 * Tile::TILE_SIZE),
                                              (1 * Tile::TILE_SIZE)));
            level.addEntity(new Flyer(level, (1 * Tile::TILE_SIZE),
                                              (1 * Tile::TILE_SIZE)));
        }
        left = !left;
    }
    if(waveTimer == 60 * 5)
    {
        level.addEntity(new Powerup(level, 12.5 * Tile::TILE_SIZE - 3, 10 * Tile::TILE_SIZE));
    }
    if(waveTimer > 60 * 15)
    {
        allWaveEnemiesSpawned = true;
    }
}
void Game::doWaveLogicF()
{
    static bool left = false;

    if(waveTimer == 0)
    {
        left = true;
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 10 * Tile::TILE_SIZE});
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 10 * Tile::TILE_SIZE});
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
    }

    if(waveTimer % (20) == 0)
    {
        if(left)
        {
            Zombie* zombie = new Zombie(level, (1 * Tile::TILE_SIZE),
                                              (17 * Tile::TILE_SIZE));
            zombie->shield = true;
            level.addEntity(zombie);

            Lunger* lunger = new Lunger(level, (23 * Tile::TILE_SIZE),
                                              (10 * Tile::TILE_SIZE));
            level.addEntity(lunger);
        }
        else
        {
            Lunger* lunger = new Lunger(level, (1 * Tile::TILE_SIZE),
                                              (10 * Tile::TILE_SIZE));
            level.addEntity(lunger);
            Zombie* zombie = new Zombie(level, (23 * Tile::TILE_SIZE),
                                              (17 * Tile::TILE_SIZE));
            zombie->shield = true;
            level.addEntity(zombie);
        }
        left = !left;
    }

    if(waveTimer == 60)
    {
        level.addEntity(new Powerup(level, 12.5 * Tile::TILE_SIZE - 3, 10 * Tile::TILE_SIZE));
    }
    if(waveTimer > 100)
    {
        allWaveEnemiesSpawned = true;
    }
}
void Game::doWaveLogicG()
{
    if(waveTimer == 0)
    {
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 10 * Tile::TILE_SIZE});
        spawnLocations.push_back({12 * Tile::TILE_SIZE, 9 * Tile::TILE_SIZE});
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 10 * Tile::TILE_SIZE});
        spawnLocations.push_back({12 * Tile::TILE_SIZE, 1 * Tile::TILE_SIZE});
    }
    if(waveTimer % (240) == 0)
    {
        level.addEntity(new Lunger(level, (1 * Tile::TILE_SIZE),
                                          (10 * Tile::TILE_SIZE)));

        level.addEntity(new Lunger(level, (12 * Tile::TILE_SIZE),
                                          (9 * Tile::TILE_SIZE)));

        level.addEntity(new Lunger(level, (23 * Tile::TILE_SIZE),
                                          (10 * Tile::TILE_SIZE)));
    }
    if(waveTimer % 300 == 0)
    {
        level.addEntity(new Flyer(level, (12.f * Tile::TILE_SIZE), (1 * Tile::TILE_SIZE)));
    }
    if(waveTimer == 60 * 5)
    {
        level.addEntity(new Powerup(level, 12.5 * Tile::TILE_SIZE - 3, 10 * Tile::TILE_SIZE));
    }
    if(waveTimer > 60 * 15)
    {
        allWaveEnemiesSpawned = true;
    }
}
void Game::doWaveLogicH()
{
    if(waveTimer == 0)
    {
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 1 * Tile::TILE_SIZE});
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 1 * Tile::TILE_SIZE});
    }
    if(waveTimer % (180) == 0)
    {
        level.addEntity(new Suicider(level, (23 * Tile::TILE_SIZE),
                                          (17 * Tile::TILE_SIZE)));
        level.addEntity(new Suicider(level, (23 * Tile::TILE_SIZE),
                                          (1 * Tile::TILE_SIZE)));

        level.addEntity(new Suicider(level, (1 * Tile::TILE_SIZE),
                                          (17 * Tile::TILE_SIZE)));
        level.addEntity(new Suicider(level, (1 * Tile::TILE_SIZE),
                                          (1 * Tile::TILE_SIZE)));
    }

    if(waveTimer == 60 * 5)
    {
        level.addEntity(new Powerup(level, 12.5 * Tile::TILE_SIZE - 3, 10 * Tile::TILE_SIZE));
    }
    if(waveTimer > 60 * 15)
    {
        allWaveEnemiesSpawned = true;
    }
}

void Game::doWaveLogicI()
{
    if(waveTimer == 0)
    {
        spawnLocations.push_back({23 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
        spawnLocations.push_back({1 * Tile::TILE_SIZE, 17 * Tile::TILE_SIZE});
    }
    static int counter = 0;
    if(waveTimer == 0)
    {
        counter = 0;
    }
    if(waveTimer % (120) == 0)
    {
        switch(counter)
        {
        case 0:
            level.addEntity(new Zombie(level, (1 * Tile::TILE_SIZE),
                                              (17 * Tile::TILE_SIZE)));
            level.addEntity(new Zombie(level, (23 * Tile::TILE_SIZE),
                                              (17 * Tile::TILE_SIZE)));
            break;
        case 1:

            level.addEntity(new Lunger(level, (1 * Tile::TILE_SIZE),
                                              (17 * Tile::TILE_SIZE)));
            level.addEntity(new Lunger(level, (23 * Tile::TILE_SIZE),
                                              (17 * Tile::TILE_SIZE)));
            break;
        case 2:
            level.addEntity(new Flyer(level, (1 * Tile::TILE_SIZE),
                                              (17 * Tile::TILE_SIZE)));
            level.addEntity(new Flyer(level, (23 * Tile::TILE_SIZE),
                                              (17 * Tile::TILE_SIZE)));
            break;
        case 3:
            level.addEntity(new Suicider(level, (1 * Tile::TILE_SIZE),
                                              (17 * Tile::TILE_SIZE)));
            level.addEntity(new Suicider(level, (23 * Tile::TILE_SIZE),
                                              (17 * Tile::TILE_SIZE)));
            break;
        }
        counter = (counter + 1)%4;
    }
    if(waveTimer == 60 * 5)
    {
        level.addEntity(new Powerup(level, 12.5 * Tile::TILE_SIZE - 3, 10 * Tile::TILE_SIZE));
    }
    if(waveTimer > 60 * 18)
    {
        allWaveEnemiesSpawned = true;
    }
}

void Game::doWaveLogicJ()
{
    if(waveTimer == 0)
    {
        allWaveEnemiesSpawned = true;
        Boss* boss = new Boss(level, 12.f * Tile::TILE_SIZE, Tile::TILE_SIZE * 2);
        level.addEntity(boss);
    }
}

void Game::gameWonLogic()
{
    if(waveTimer == 0)
    {
        ResourceManager::stopSong("boss.ogg");
    }
    if(waveTimer == 300)
    {
        level.openTop();
        level.shake(10);
    }
    if(waveTimer > 400)
    {
        /// Vacuum begins
        // 10 * Tile::TILE_SIZE, -16
        sf::Vector2f delta({10 * Tile::TILE_SIZE - (player->x+player->w/2),
                            -Tile::TILE_SIZE - (player->y + player->h / 2)});

        float magnitude = sqrtf(delta.x * delta.x + delta.y * delta.y);

        player->dx += 2 * Entity::GRAVITY * delta.x / magnitude;
        player->dy += 2 * Entity::GRAVITY * delta.y / magnitude;
        if(player->isOutOfBounds())
        {
            endCinematic = true;
        }
    }
}

void Game::step()
{

    if(!RAGNAROK)
    {
        time++;
    }
    if(configuring)
    {
        configureStep();
        return;
    }
    if(endCinematic)
    {
        endingStep();
        return;
    }
    prevEscape = escape;
    escape = sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
    if(escape && !prevEscape)
    {
        paused = !paused;
    }
    if(paused)
    {
        return;
    }
    if(gameStarted)
    {
        if(gameStartTimer > 0)
        {
            gameStartTimer--;
            ResourceManager::setMusicVolume("bgm.ogg", 50 - 50 * (gameStartTimer / (float)GAME_START_TIME));
        }
        else if(waveCoolDownTimer > 0)
        {
            waveCoolDownTimer--;
            if(currWave == 9)
            {
                handleBossMusicFade();
            }
        }
        else if(!allWaveEnemiesSpawned)
        {
            switch(currWave)
            {
            case 0:
                doWaveLogicA();
                break;
            case 1:
                doWaveLogicB();
                break;
            case 2:
                doWaveLogicC();
                break;
            case 3:
                doWaveLogicD();
                break;
            case 4:
                doWaveLogicE();
                break;
            case 5:
                doWaveLogicF();
                break;
            case 6:
                doWaveLogicG();
                break;
            case 7:
                doWaveLogicH();
                break;
            case 8:
                doWaveLogicI();
                break;
            case 9:
                doWaveLogicJ();
                break;
            case 10:
                gameWonLogic();
                break;
            default:
                printf("No wave logic defined for %d\n", currWave);
                break;
            }
            waveTimer++;
        }
    }
    else
    {
        if(player->hasArrow)
        {
            {
                ResourceManager::playSong("bgm.ogg");
                gameStarted = true;
                gameStartTimer = GAME_START_TIME;
                level.closeTop();
                level.shake(20);
            }
        }
    }
    if(player->dead && deathAnimTimer > 0)
    {
        deathAnimTimer--;
    }

    doInput();
    static int testX, testY, prevTestX, prevTestY;

    prevTestX = testX;
    prevTestY = testY;
    testX = (player->x + player->w/2)/ Tile::TILE_SIZE;
    testY = (player->y + player->h/2)/ Tile::TILE_SIZE;
    if(testX != prevTestX || testY != prevTestY)
    {
        //printf("%d, %d\n", testX, testY);
    }
    unsigned enemyCount = 0;
    for(unsigned i = 0; i < entities.size(); i++)
    {
        entities[i]->step();
        if(entities[i]->enemy)
        {
            if(RAGNAROK)
            {
                entities[i]->die = true;
            }
            enemyCount++;
        }
        if(entities[i]->entityType == Entity::TYPE_PROJECTILE &&
           static_cast<Projectile*>(entities[i])->ragnarok)
        {
            RAGNAROK = true;
        }
        /// if an enemy is out of bounds, delete it. just in case this ever happens... (it hasnt yet)
        if(entities[i]->isOutOfBounds() && entities[i]->enemy && entities[i]->entityType != Entity::TYPE_BOSS)
        {
            entities[i]->die = true;
        }
        for(unsigned j =0 ; j < entities.size(); j++)
        {
            if(i == j)
            {
                continue;
            }
            if(entities[i]->isCollidingWith(entities[j]))
            {
                entities[i]->collideEntity(entities[j]);
            }
        }
    }
    if(allWaveEnemiesSpawned && enemyCount == 0)
    {
        nextWave();
    }
    vector<Entity*>::iterator ent_it = entities.begin();
    for(;ent_it != entities.end(); ent_it++)
    {
        if((*ent_it)->die)
        {
            vector<Entity*>::iterator prev_it = ent_it;
            prev_it--;
            delete (*ent_it);
            entities.erase(ent_it);
            ent_it = prev_it;
        }
    }
}


void Game::draw(sf::RenderWindow& window)
{
    if(configuring)
    {
        configureDraw(window);
        return;
    }
    if(endCinematic)
    {
        endingDraw(window);
        return;
    }
    if(keys[sf::Keyboard::F1] && !prevKeys[sf::Keyboard::F1])
    {
        slowMotion = !slowMotion;
        if(slowMotion)
        {
            window.setFramerateLimit(5);
        }
        else
        {
            window.setFramerateLimit(60);
        }
    }
    //view.setCenter((int)(player->x), (int)(player->y));
    ///Shake FX time?
    if(level.shakeTimer > 0)
    {
        view.setCenter({12.5f * Tile::TILE_SIZE + (rand()/(float)RAND_MAX * 2.f * level.shakeAmount - level.shakeAmount),
                       9.5f * Tile::TILE_SIZE + (rand()/(float)RAND_MAX * 2.f * level.shakeAmount - level.shakeAmount)});
        level.shakeTimer--;
    }
    else
    {
        view.setCenter({12.5f * Tile::TILE_SIZE, 9.5f * Tile::TILE_SIZE});
    }
    window.setView(view);
    level.draw(window);
    sf::RectangleShape spawn({Tile::TILE_SIZE, Tile::TILE_SIZE});
    spawn.setFillColor(sf::Color(255, 0, 0, 100));
    spawn.setOrigin(Tile::TILE_SIZE/2, Tile::TILE_SIZE/2);

    for(unsigned i = 0; i < spawnLocations.size(); i++)
    {
        spawn.setRotation(rand()%360);
        spawn.setScale(0.5 + 0.5 * rand()/(float)RAND_MAX, 0.5 + 0.5 * rand()/(float)RAND_MAX);
        spawn.setPosition(spawnLocations[i] + sf::Vector2f(Tile::TILE_SIZE/2, Tile::TILE_SIZE/2));
        window.draw(spawn);
    }
    for(unsigned i = 0; i < entities.size(); i++)
    {
        entities[i]->draw(window);
        if(entities[i]->shield)
        {
            entities[i]->drawShield(window);
        }
    }
    if(gameStarted)
    {
        if(waveCoolDownTimer > 0)
        {
            drawWaveGui(window);
        }
         if(titleAlpha > 0)
        {
            titleAlpha-=4;
        }
    }
    if(titleAlpha > 0)
    {
        drawTitleScreen(window);
    }
    if(player->dead)
    {
        drawDeathOverlay(window);
    }
    if(paused)
    {
        sf::RectangleShape overlay({400, 300});
        overlay.setFillColor(sf::Color(0, 0, 0, 100));
        overlay.setPosition(0, 0);
        window.draw(overlay);
    }
}

void Game::configureStep()
{
    input.step();
    if(!input.binding)
    {
        configureExitTimer++;
        if(configureExitTimer >= CONFIGURE_EXIT_TIME)
        {
            configuring = false;
        }
    }

    for(unsigned i = 0; i < fallingParticles.size(); i++)
    {
        fallingParticles[i].y -= (input.binding ? 4 : 3.5);
        if(fallingParticles[i].y < 0)
        {
            fallingParticles.erase(fallingParticles.begin() + i);
            i--;
        }
    }
    if(fallingParticles.size() < 40)
    {
        fallingParticles.push_back({9 * Tile::TILE_SIZE + (float)(rand()%(Tile::TILE_SIZE * 2)), 300});
    }
}

void Game::configureDraw(sf::RenderWindow& window, bool drawFalling)
{
    if(playerFallSprite == NULL)
    {
        playerFallSprite = new Sprite(ResourceManager::getTexture("player_jumpfall.png"), 16, 16, -1, 0, 0, 1, false);
    }
    frame++;
    window.setView(view);
    view.setCenter(200, 150);
    sf::RectangleShape box({400, 300});
    box.setPosition(0, 0);
    box.setFillColor(sf::Color(78, 74, 78));
    window.draw(box);

    sf::RectangleShape passage({32, 300});
    passage.setPosition(9 * Tile::TILE_SIZE, 0);
    passage.setFillColor(sf::Color(222, 238, 214));
    window.draw(passage);
    if(drawFalling)
    {
        /// draw dust particles
        sf::RectangleShape particle({1, 1});
        particle.setFillColor(sf::Color(50, 60, 100));
        for(unsigned i = 0; i < fallingParticles.size(); i++)
        {
            particle.setPosition(fallingParticles[i]);
            window.draw(particle);
        }


        playerFallSprite->setFrame(1);

        int offsetY = 100.f * powf((configureExitTimer / (float)CONFIGURE_EXIT_TIME), 2);

        playerFallSprite->setPosition(159 + cosf(frame / 40.f) * 5.f, offsetY + 250 + sinf(frame/25.f) * 4.f);
        playerFallSprite->draw(window);
    }
/*
            ACTION_LEFT = 0,
            ACTION_RIGHT = 1,
            ACTION_JUMP = 2,
            ACTION_SHOOT = 3,
*/
    /// actually draw the configuration stuff
    keyboardSprite.setPosition(196, 150);
    gamepadSprite.setPosition(298, 150);
    keyboardSprite.draw(window);
    gamepadSprite.draw(window);

    if(!input.joystickDetected)
    {
        sf::RectangleShape greyOutBox({98, 20});
        greyOutBox.setFillColor(sf::Color(0, 0, 0, 120));
        greyOutBox.setPosition(298, 150);
        window.draw(greyOutBox);
    }
    sf::RectangleShape keyForeGround({98, 20});
    keyForeGround.setFillColor(sf::Color(0, 0, 0, 100));

    sf::Text chosenText("", ResourceManager::getDefaultFont(), 8);
    chosenText.setColor(sf::Color::White);
    if(input.controllerChosen)
    {
        keyForeGround.setSize({98, 20});
    }
    else
    {
        keyForeGround.setSize({98.f * input.controllerChooseTimer / Input::BIND_TIME, 20});
    }
    if(!input.usingJoystick)
    {
        keyForeGround.setPosition(196, 150);
        chosenText.setString("Using Keyboard");
        chosenText.setPosition(224, 150);
    }
    else
    {
        keyForeGround.setPosition(298, 150);
        stringstream ss("");
        ss<<"Using Gamepad "<<input.joystickId;
        chosenText.setString(ss.str());
        chosenText.setPosition(323, 150);
    }
    window.draw(keyForeGround);
    window.draw(chosenText);




    sf::Text bindText;
    bindText.setFont(ResourceManager::getDefaultFont());
    bindText.setCharacterSize(8);
    bindText.setColor(sf::Color::White);

    sf::RectangleShape keyBackground({200, 20});
    keyBackground.setFillColor(sf::Color(133, 149, 161));
    for(unsigned i = 0; i < input.keys.size(); i++)
    {
        keyBackground.setPosition( 196, 175 + i * 25);
        keyForeGround.setPosition(196, 175 + i * 25);
        bindText.setPosition(221, 175 + i * 25);
        Sprite* toDraw;

        if(i == 0)
        {
            toDraw = &leftSprite;
        }
        else if(i == 1)
        {
            toDraw = &downSprite;
        }
        else if(i == 2)
        {
            toDraw = &rightSprite;
        }
        else if(i == 3)
        {
            toDraw = &jumpSprite;
        }
        else if(i == 4)
        {
            toDraw = &throwSprite;
        }

        window.draw(keyBackground);
        if((int)i < input.currentlyBinding || ((!input.binding) && input.controllerChosen))
        {
            keyForeGround.setSize({200, 20});
            window.draw(keyForeGround);
            bindText.setString("BOUND");
            window.draw(bindText);
        }
        else if((int)i == input.currentlyBinding && input.controllerChosen)
        {

            keyForeGround.setSize({200 * (input.bindTimer / (float)Input::BIND_TIME), 20});
            window.draw(keyForeGround);
            if(i == 0)
            {
                bindText.setString("Hold down the key to use for Left");
            }
            else if(i == 1)
            {
                bindText.setString("Hold down the key to use for Down");
            }
            else if(i == 2)
            {
                bindText.setString("Hold down the key to use for Right");
            }
            else if(i == 3)
            {
                bindText.setString("Hold down the key to use for Jump");
            }
            else if(i == 4)
            {
                bindText.setString("Hold down the key to use for Shoot");
            }
            window.draw(bindText);
        }
        toDraw->setPosition(196, 175 + i * 25);
        toDraw->draw(window);
    }

    /// draw title
    // THE HALLOWED ORB
    titleSprite.setPosition(196, 20);
    titleSprite.draw(window);

}

void Game::endingStep()
{
    if(!escaped)
    {
        endingPlayerY -= 6;
        if(endingPlayerY < -10)
        {
            escaped = true;
            view.setSize(800, 600);
            view.setCenter(400, 300);
            endingPlayerY = 600;

        }
    }
    else
    {
        if(!finalCutscene)
        {
            endingPlayerY -= 4;
            if(endingPlayerY < -4)
            {
                finalCutscene = true;
                fallingParticles.clear();
                ResourceManager::playSong("ending.ogg");
            }
        }
        else
        {
            while(fallingParticles.size() < 100)
            {
                fallingParticles.push_back({(float)(rand()%800), (float)(rand()%600)});
            }
            endingPlayerY++;
        }
    }
}
void Game::endingDraw(sf::RenderWindow& window)
{

    if(!escaped)
    {
        configureDraw(window, false);
        /// draw player flying up here
        playerFallSprite->setFrame(0);
        frame++;
        playerFallSprite->setPosition(159 + cosf(frame/9.f) * 8, endingPlayerY);
        playerFallSprite->draw(window);
    }
    else
    {
        if(!finalCutscene)
        {
            window.setView(view);

            sf::Vertex rectangle[] =
            {
                sf::Vertex({0, 0}, sf::Color::Black),
                sf::Vertex({800, 0}, sf::Color::Black),
                sf::Vertex({800, 600-16}, sf::Color(109, 194, 202)),
                sf::Vertex({0, 600-16}, sf::Color(109, 194, 202)),
            };
            window.draw(rectangle, 4, sf::Quads); //sky

            sf::RectangleShape land({400 - 16,16});
            land.setFillColor(sf::Color(78, 74, 78));
            land.setPosition(0, 600 - 16);
            window.draw(land);
            land.setPosition(400 + 16, 600 - 16);
            window.draw(land);


            playerFallSprite->setFrame(0);
            frame++;
            playerFallSprite->setPosition(400 + cosf(frame/9.f) * 3, endingPlayerY);
            playerFallSprite->draw(window);
        }
        else
        {
            view.setSize(800, 600);
            view.setCenter(400, 300);
            window.setView(view); //??
            sf::Vertex rectangle[] =
            {
                sf::Vertex({0, 0}, sf::Color::Black),
                sf::Vertex({800, 0}, sf::Color::Black),
                sf::Vertex({800, 600}, sf::Color(0, 0, 10)),
                sf::Vertex({0, 600}, sf::Color(0, 0, 10)),
            };
            window.draw(rectangle, 4, sf::Quads); ///space


            ///draw stars
            for(unsigned i = 0; i < fallingParticles.size(); i++)
            {
                sf::RectangleShape star({2, 2});
                star.setFillColor(sf::Color(78, 74, 78));
                star.setPosition(fallingParticles[i]);
                window.draw(star);
            }

            sf::CircleShape planet(100, 50);
            planet.setPosition(400 - 100, 300 - 100);
            planet.setFillColor(sf::Color(78, 74, 78));
            window.draw(planet);


            sf::RectangleShape playerBody({1, 1});
            playerBody.setFillColor(sf::Color::White);
            playerBody.setPosition(400, 300-100-endingPlayerY);
            window.draw(playerBody);

            if(endingPlayerY > 200)
            {
                stringstream ss("");
                ss<<"You did it!\n"<<"Your score: "<< 100 * ((player->health - player->maxHealth) + 5) <<"\nYour time: "<<(int)(time/60)<<" seconds.";
                sf::Text endingText;
                endingText.setFont(ResourceManager::getDefaultFont());
                endingText.setString(ss.str());
                endingText.setCharacterSize(16);
                endingText.setColor(sf::Color::White);
                endingText.setPosition( 350, 40);
                window.draw(endingText);
            }
        }
    }
}
void Game::drawTitleScreen(sf::RenderWindow& window)
{
//    sf::Text titleText;
//    titleText.setFont(ResourceManager::getDefaultFont());
//    titleText.setPosition(40, 40);
//    titleText.setString("unnamed");
//    titleText.setColor(sf::Color(0, 0, 0, titleAlpha));
//    titleText.setCharacterSize(24);
//    window.draw(titleText);
//    //subtext? controls
//    titleText.setCharacterSize(8);
//    titleText.setPosition(40, 70);
//    window.draw(titleText);
}

void Game::drawWaveGui(sf::RenderWindow& window)
{
    if(currWave == 9) ///Boss
    {
        ///draw s omething special
        sf::Text waveText;
        waveText.setFont(ResourceManager::getDefaultFont());
        waveText.setPosition( 25, 40);
        waveText.setString("WARNING: BOSS APPROACHING");
        if((WAVE_COOLDOWN_TIME - waveCoolDownTimer) % 120 == 1)
        {
            ResourceManager::playSound("boss_horn.wav");
        }
        waveText.setColor(sf::Color(255, 0, 0, 127 + sinf(PI * (WAVE_COOLDOWN_TIME - waveCoolDownTimer) / 60.f - (PI/2)) * 127 + 1));
        waveText.setCharacterSize(24);
        window.draw(waveText);

        level.shake(10, 2);
    }
    else if(currWave < 9)
    {
        sf::RectangleShape barBg({200, 5});
        sf::RectangleShape barFg({198 * (waveCoolDownTimer / (float)WAVE_COOLDOWN_TIME), 3});

        barFg.setFillColor(sf::Color::Cyan);
        barBg.setFillColor(sf::Color::Black);

        barBg.setPosition (100, 60);
        barFg.setPosition(barBg.getPosition() + sf::Vector2f(1, 1));
        window.draw(barBg);
        window.draw(barFg);

        stringstream ss("");
        ss << "Wave " << (currWave+1);
        sf::Text waveText;
        waveText.setFont(ResourceManager::getDefaultFont());
        waveText.setPosition( 186, 47);
        waveText.setString(ss.str());
        waveText.setColor(sf::Color::Black);
        waveText.setCharacterSize(8);
        window.draw(waveText);
    }
}

void Game::drawDeathOverlay(sf::RenderWindow& window)
{
    float percent = deathAnimTimer / (float)DEATH_ANIM_TIME;
    float transitionRadius = 50 + 300.f * deathAnimTimer / (float)DEATH_ANIM_TIME;
    overlayTexture.clear(sf::Color::Black);
    sf::CircleShape circle(transitionRadius);
    circle.setPosition(player->x + player->w/2 - transitionRadius,
                       player->y + player->h/2 - transitionRadius);
    circle.setFillColor(sf::Color(0, 0, 0, 0));
    overlayTexture.draw(circle, sf::BlendNone);
    overlayTexture.display();
    sf::Sprite sprite(overlayTexture.getTexture());
    window.draw(sprite);

    stringstream ss("");
    ss<<"You died on wave "<<(currWave+1)<<"\nPress SHOOT or JUMP to try again!";

    sf::Vector2f position;
    if(player->x < level.w * Tile::TILE_SIZE / 2)
    {
        if(player->y < level.h * Tile::TILE_SIZE / 2)
        {
            position = { 200, 160 };
        }
        else
        {
            position = { 200, 50 };
        }
    }
    else
    {
        if(player->y < level.h * Tile::TILE_SIZE / 2)
        {
            position = { 50, 160 };
        }
        else
        {
            position = { 50, 50 };
        }
    }


    sf::Text gameOverText;
        gameOverText.setFont(ResourceManager::getDefaultFont());
        gameOverText.setCharacterSize(24);
        gameOverText.setString("Game Over");
        gameOverText.setPosition(position);
        gameOverText.setColor(sf::Color::White);
    window.draw(gameOverText);
        gameOverText.setCharacterSize(8);
        gameOverText.setString(ss.str());
        gameOverText.setPosition(position.x, position.y + 40);
    window.draw(gameOverText);
    if(deathAnimTimer == DEATH_ANIM_TIME-1)
    {
        ResourceManager::playSong("static.ogg");
    }
    ResourceManager::setCurrMusicVolume(50.f * percent);
    ///ResourceManager::setMusicVolume("bgm.ogg", 50.f * percent);
    ResourceManager::setMusicVolume("static.ogg", 50 - 50.f * percent);
}
void Game::doInput()
{
    input.step();

    if(!player->dead)
    {
        if(input.isHeld(Input::ACTION_LEFT))
        {
            player->moveLeft();
        }

        if(input.isPressed(Input::ACTION_DOWN))
        {
            player->tryMuteMusic();
        }

        if(input.isHeld(Input::ACTION_RIGHT))
        {
            player->moveRight();
        }

        if(input.isHeld(Input::ACTION_JUMP))
        {
            player->jump();
        }

        if(input.isPressed(Input::ACTION_SHOOT))
        {
            player->startShoot();
        }

        if(input.isReleased(Input::ACTION_SHOOT))
        {
            player->stopShoot();
        }
    }
    else
    {
        if(input.isPressed(Input::ACTION_SHOOT) ||
           input.isPressed(Input::ACTION_JUMP))
        {
            if(player->landedDown && deathAnimTimer == 0)
            {
                newGame();
            }
        }

    }

}

void Game::handleBossMusicFade()
{
    if(waveCoolDownTimer == WAVE_COOLDOWN_TIME/2)
    {
        ResourceManager::setMusicVolume("boss.ogg", 0);
        ResourceManager::stopSong("bgm.ogg");
        ResourceManager::playSong("boss.ogg");
    }
    if(waveCoolDownTimer > WAVE_COOLDOWN_TIME / 2)
    {
        float bgmPercent = (waveCoolDownTimer - (WAVE_COOLDOWN_TIME/2)) / (float)(WAVE_COOLDOWN_TIME/2) ;
        ResourceManager::setMusicVolume("bgm.ogg", 50 * bgmPercent);
    }
    else
    {
        float bossPercent = 1 - waveCoolDownTimer / (float)(WAVE_COOLDOWN_TIME/2);
        ResourceManager::setMusicVolume("boss.ogg", bossPercent * 50);
    }
}
