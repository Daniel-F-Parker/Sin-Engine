#include "Tilemap.h"
Tilemap::Tilemap()
{

}
Tilemap::Tilemap(int* tiles)
{
    this->tiles = tiles;
}
Tilemap::~Tilemap()
{
    delete(this);
}
