#pragma once
class V2
{
	public:
	float x;
	float y;
	
	V2 operator+(V2 val2);
	V2 operator-(V2 val2);
	V2 operator*(float val1);
	V2 operator*=(float val2);
	V2 operator+=(V2 val);
};

