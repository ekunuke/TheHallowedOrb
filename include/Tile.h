#ifndef TILE_H
#define TILE_H


class Tile
{
    public:
        static const int TILE_SIZE = 16;
        enum TileType
        {
            TILE_NONE,
            TILE_BOUNCY,
            TILE_SOLID
        };

        Tile(TileType tileType);
        ~Tile();

        TileType tileType;

        bool solid;

        bool left;
        bool right;
        bool up;
        bool down;
};

#endif // TILE_H
