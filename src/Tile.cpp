#include "Tile.h"

Tile::Tile(TileType tileType):
    tileType(tileType),
    solid(false),

    left(false),
    right(false),
    up(false),
    down(false)
{
    //ctor
}

Tile::~Tile()
{
    //dtor
}
