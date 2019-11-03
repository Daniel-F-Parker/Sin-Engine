#pragma once
#include <vector> 
class Tilemap
{
    public:    
	int* tiles;

    Tilemap();
    Tilemap(int* tiles);
    ~Tilemap();
};