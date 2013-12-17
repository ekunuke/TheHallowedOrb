#ifndef GAME_H
#define GAME_H

#include "toolbox.h"
#include "ResourceManager.h"
#include "Level.h"
#include "Input.h"
#include "Sprite.h"

class Entity;
class Player;

class Game
{
    public:
        Game();
        ~Game();

        void step();
        void draw(sf::RenderWindow& window);
        void configureStep();
        void configureDraw(sf::RenderWindow& window, bool drawFalling = true);
        void endingStep();
        void endingDraw(sf::RenderWindow& window);

    private:
        int frame;
        int time;
        Sprite downSprite;
        Sprite leftSprite;
        Sprite rightSprite;
        Sprite throwSprite;
        Sprite jumpSprite;

        Sprite titleSprite;
        Sprite gamepadSprite;
        Sprite keyboardSprite;
        Sprite *playerFallSprite;
        Input input;
        sf::RenderTexture overlayTexture;
        sf::View view;
        Level level;
        Player* player;

        vector<Entity*> entities;
        vector<bool> keys;
        vector<bool> prevKeys;
        void drawTitleScreen(sf::RenderWindow& window);
        void drawWaveGui(sf::RenderWindow& window);
        void drawDeathOverlay(sf::RenderWindow& window);
        void nextWave();

        void doWaveLogicA();
        void doWaveLogicB();
        void doWaveLogicC();
        void doWaveLogicD();
        void doWaveLogicE();

        void doWaveLogicF();
        void doWaveLogicG();
        void doWaveLogicH();
        void doWaveLogicI();
        void doWaveLogicJ();

        void gameWonLogic();

        void newGame();
        void doInput();
        void handleBossMusicFade();

        static const int WAVE_COOLDOWN_TIME = 3 * 60;
        static const int DEATH_ANIM_TIME = 120; // 2 second
        static const int GAME_START_TIME = 60; // 2 second
        static const int CONFIGURE_EXIT_TIME = 60; // 2 second

        vector<sf::Vector2f> fallingParticles;
        vector<sf::Vector2f> spawnLocations;
        int waveCoolDownTimer;
        int waveTimer;
        int currWave;
        bool allWaveEnemiesSpawned;

        int endingPlayerY;
        bool endCinematic;
        bool finalCutscene;
        bool escaped;
        bool configuring;
        int configureExitTimer;
        bool gameStarted;
        int titleAlpha;

        int gameStartTimer;
        int deathAnimTimer;

        bool slowMotion;
        bool prevEscape;
        bool escape;
        bool paused;

        bool RAGNAROK;
};

#endif // GAME_H
