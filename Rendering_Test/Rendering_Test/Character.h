#pragma once
#include "Vector2.h"
class Character
{
public:
	V2 position;
	int tile_x;
	int tile_y;

	Character();
	Character(V2 position, int tile_x, int tile_y);
	~Character();

	int Move(V2 velocity);
	int Draw();
	void SetPosition(V2 position);
	V2 GetPosition();
};



