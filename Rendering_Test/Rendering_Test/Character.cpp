
#include "Character.h"


Character::Character()
{
	position = { 0,0 };
	tile_x = 0;
	tile_y = 0;
}

Character::Character(V2 position, int tile_x, int tile_y)
{
	this->position = position;
	this->tile_x = tile_x;
	this->tile_y = tile_y;
}

Character::~Character()
{
}

int Character::Move(V2 velocity)
{
	position += velocity;
	return 0;
}

int Character::Draw()
{
	return 0;
}

void Character::SetPosition(V2 position)
{
	this->position = position;
}

V2 Character::GetPosition()
{
	return this->position;
}