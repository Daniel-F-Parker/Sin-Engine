#include "Vector2.h"

V2 V2::operator+(V2 val2)
{
	V2 result;
	result.x = this->x + val2.x;
	result.y = this->y + val2.y;

	return result;
}
V2 V2::operator-(V2 val1)
{
	V2 result;
	result.x = this->x-val1.x;
	result.y = this->y-val1.y;

	return result;
}

V2 V2::operator*(float val1)
{
	V2 result;
	result.x = val1 * this->x;
	result.y = val1 * this->y;

	return result;
}

V2 V2::operator*=(float val)
{
    *this = *this*val;
	return *this;
}
V2 V2::operator+=(V2 val)
{
	*this = *this + val;

	return *this;
}

// float DotProduct(V2 val1, V2 val2)
// {
// 	float result = val1.x*val2.x + val1.y*val2.y;
// 	return result;
// }

// float MagnitudeSqrd(V2 val)
// {
// 	float result = DotProduct(val, val);
// 	return result;
// }
