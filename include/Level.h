#ifndef LEVEL_H
#define LEVEL_H

#include "toolbox.h"
#include "Tile.h"

class Entity;
class Player;

class Level
{
    public:
        Level(unsigned w, unsigned h);
        ~Level();

        void draw(sf::RenderWindow& window);
        Tile& getTile(unsigned x, unsigned y);
        void setEntities(vector<Entity*> *newEntities);
        void setPlayer(Player* p);
        Player* getPlayer();
        void addEntity(Entity* entity);
        void buildLevel();
        void shake(int time, int amount = 2);
        void openTop(bool playSound = true);
        void closeTop(bool playSound = true);
    private:
        void placeTileEntities();
        void processTileCollision();
    public:
        unsigned w;
        unsigned h;
    private:
        vector<vector<Tile>> tiles;
        vector<Entity*> *entities;
        Player* player;
    public:
        int shakeTimer;
        int shakeAmount;
};

#endif // LEVEL_H
