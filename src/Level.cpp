#include "Level.h"
#include "Entity.h"
#include "Player.h"
#include "Bouncy.h"
#include "Pedestal.h"
#include "Projectile.h"

Level::Level(unsigned w, unsigned h):
    w(w),
    h(h),
    entities(NULL),
    player(NULL),
    shakeTimer(0)
{
    tiles.resize(w, vector<Tile>());
    for(unsigned i = 0; i < w; i++)
    {
        tiles[i].resize(h, Tile(Tile::TILE_NONE));
    }
}

Level::~Level()
{
    //dtor
}

void Level::draw(sf::RenderWindow& window)
{
    sf::RectangleShape tileBox({Tile::TILE_SIZE, Tile::TILE_SIZE});
    tileBox.setFillColor(sf::Color(78, 74, 78));

    for(unsigned x = 0; x < w; x++)
    {
        for(unsigned y = 0; y < h; y++)
        {
            if(tiles[x][y].tileType == Tile::TILE_SOLID)
            {
                tileBox.setPosition(x * Tile::TILE_SIZE, y * Tile::TILE_SIZE);
                window.draw(tileBox);
            }
        }
    }
}

Tile& Level::getTile(unsigned x, unsigned y)
{
    return tiles[x][y];
}


void Level::setEntities(vector<Entity*> *newEntities)
{
    entities = newEntities;
}

void Level::setPlayer(Player* p)
{
    player = p;
}

Player* Level::getPlayer()
{
    return player;
}

void Level::addEntity(Entity* entity)
{
    assert(entities);
    entities->push_back(entity);
}

void Level::buildLevel()
{
    for(unsigned x = 0; x < w; x++)
    {
        for(unsigned y = 0; y < h; y++)
        {
            if(x == 0 ||
               y == 0 ||
               x == w-1 ||
               y == h-1 ||
               (x % 10 > 5 && y > h - 4) ||
               (x < 5 && y < h-5 && y > h-9) ||
               (x > 7 && y < h - 11 && y > h - 13))
            {
                tiles[x][y].tileType = Tile::TILE_SOLID;
            }

        }
    }
    tiles[6][15].tileType = Tile::TILE_BOUNCY;
    tiles[4][10].tileType = Tile::TILE_BOUNCY;
    tiles[12][17].tileType = Tile::TILE_BOUNCY;
    /// mirror
    //25 / 2 = 12
    for(unsigned x = 0; x < w/2; x++)
    {
        for(unsigned y = 0; y < h; y++)
        {
            tiles[x + w/2 ][y].tileType = tiles[w/2 - x][y].tileType;
        }
    }
    placeTileEntities();
    processTileCollision();
}

void Level::shake(int time, int amount)
{
    shakeTimer = time;
    shakeAmount = amount;
}

void Level::openTop(bool playSound)
{
    for(unsigned x = 9; x <= 10; x++)
    {
        tiles[x][0].tileType = Tile::TILE_NONE;
    }
    if(playSound)
    {
        ResourceManager::playSound("level_change.wav");
    }
    processTileCollision();
}

void Level::closeTop(bool playSound)
{
    for(unsigned x = 9; x <= 10; x++)
    {
        tiles[x][0].tileType = Tile::TILE_SOLID;
    }
    if(playSound)
    {
        ResourceManager::playSound("level_change.wav");
    }
    processTileCollision();
}

void Level::placeTileEntities()
{
    for(unsigned x = 0; x < w; x++)
    {
        for(unsigned y = 0; y < h; y++)
        {
            if(tiles[x][y].tileType == Tile::TILE_BOUNCY)
            {
                tiles[x][y].tileType = Tile::TILE_NONE;
                addEntity(new Bouncy((*this), x * Tile::TILE_SIZE, y * Tile::TILE_SIZE));
            }
        }
    }
    addEntity(new Pedestal((*this), w * Tile::TILE_SIZE / 2.f - 3, 96));
    Projectile* projectile = new Projectile((*this), w * Tile::TILE_SIZE / 2.f - 3, 91.f, 0, 1);
    projectile->onPedestal = true;
    addEntity(projectile);
}
void Level::processTileCollision()
{
    for(unsigned x = 0; x < w; x++)
    {
        for(unsigned y = 0; y < h; y++)
        {
            //dont mind this
            tiles[x][y].left = false;
            tiles[x][y].right = false;
            tiles[x][y].up = false;
            tiles[x][y].down = false;
            tiles[x][y].solid = false;
            if(tiles[x][y].tileType != Tile::TILE_SOLID)
                continue;

            tiles[x][y].left = true;
            tiles[x][y].right = true;
            tiles[x][y].up = true;
            tiles[x][y].down = true;
            tiles[x][y].solid = true;

            if(x == 0 || tiles[x-1][y].tileType == Tile::TILE_SOLID)
            {
                tiles[x][y].left = false;
            }
            if(y == 0 || tiles[x][y-1].tileType == Tile::TILE_SOLID)
            {
                tiles[x][y].up = false;
            }
            if(x == w-1 || tiles[x+1][y].tileType == Tile::TILE_SOLID)
            {
                tiles[x][y].right = false;
            }
            if(y == h-1 || tiles[x][y+1].tileType == Tile::TILE_SOLID)
            {
                tiles[x][y].down = false;
            }
            if(!(tiles[x][y].left || tiles[x][y].right || tiles[x][y].up || tiles[x][y].down))
            {
                tiles[x][y].solid;
            }
        }
    }
}
