#include <stdint.h>
struct Tilemap
{
    int *tiles;
};

struct World
{
    int height;
    int width;
    int tile_size;
    Tilemap *tilemap;
};

void CreateWorld(World* world, int width, int height, int tile_size, Tilemap* tilemaps);
void CreateTilemap(int** tilemap, int width, int height, uint8_t sides);