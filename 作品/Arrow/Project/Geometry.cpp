#include "Geometry.h"
#include <cmath>
#include <algorithm>

const Vector2 Vector2::ZERO = { 0.0f,0.0f };

void Vector2::operator+=(const Vector2& val)
{
	x += val.x;
	y += val.y;
}

void Vector2::operator-=(const Vector2& val)
{
	x -= val.x;
	y -= val.y;
}

void Vector2::operator*=(float scale)
{
	x *= scale;
	y *= scale;
}

void Vector2::operator/=(float div)
{
	x /= div;
	y /= div;
}

bool Vector2::operator==(const Vector2& val)
{
	return (x == val.x && y == val.y);
}

float Vector2::Magnitude() const
{
	return std::hypotf(x, y);
}

float Vector2::SQMagnitude() const
{
	return (x * x + y * y);
}

void Vector2::Normalize()
{
	auto mag = Magnitude();
	x /= mag;
	y /= mag;
}

Vector2 Vector2::Normalized() const
{
	Vector2 ret = { x,y };
	ret.Normalize();
	return ret;
}

void Vector2::Rotate90()
{
	/*float tmpX = x;
	x = -y;
	y = tmpX;*/
	std::swap(x, y);
	x = -x;
}

Vector2 Vector2::Rotated90() const
{
	return { -y,x };
}

Vector2 operator+(const Vector2& lval, const Vector2& rval) {
	Vector2 ret = { lval.x + rval.x,lval.y + rval.y };
	return ret;
}

Vector2 operator-(const Vector2& lval, const Vector2& rval) {
	Vector2 ret = { lval.x - rval.x,lval.y - rval.y };
	return ret;
}

Vector2 operator*(const Vector2& lval, float scale)
{
	return Vector2(lval.x * scale, lval.y * scale);
}

Vector2 operator/(const Vector2& lval, float div)
{
	return Vector2(lval.x / div, lval.y / div);
}
