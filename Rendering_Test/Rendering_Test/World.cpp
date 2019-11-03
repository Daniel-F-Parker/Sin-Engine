#include "World.h"
#include <stdlib.h>
#include <stdint.h>

// int CreateTilemap(Tilemap* tilemap, int* tiles)
// {
//     tilemap->tiles = tiles;
// }

void CreateWorld(World* world, int width, int height, int tile_size, Tilemap* tilemaps)
{
    world->height = height;
    world->width = width;
    world->tile_size = tile_size;
    world->tilemap = tilemaps;
}

void CreateTilemap(int** tilemap, int width, int height, uint8_t sides)
{
    *tilemap = (int*)malloc(sizeof(int)*height*width);
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (y != 0 && x != 0 && y != (height-1) && x != (width-1))
            {
                *tilemap[y*width+x] = 1;
            }
            if ((sides >> 0) & 1)
            {
                if ((y == 0) && (y == (height/2)-1))
                    *tilemap[y*width+x] = 0;
            }
            if ((sides >> 1) & 1)
            {
                if ((y == (height-1)) && (y == (height/2)-1))
                    *tilemap[y*width+x] = 0;
            }
            if ((sides >> 2) & 1)
            {
                if ((x == 0) && (x == (width/2)-1))
                    *tilemap[y*width+x] = 0;
            }
            if ((sides >> 3) & 1)
            {
                if ((x == (width-1)) && (y == (width/2)-1))
                    *tilemap[y*width+x] = 0;
            }
            else
            {
                *tilemap[y*width+x] = 1;
            }
        }
    }
}

void DestroyTilemap(int** tilemap)
{
    free(*tilemap);
}