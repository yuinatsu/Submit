#include"Geometry.h"
#include<DxLib.h>
#include<cmath>

void
Rect::Draw() {
	DxLib::DrawBox(Left()*2, Top()*2, Right()*2, Bottom()*2, 0xffffffff, false);
}

void
Rect::Draw(Vector2_2& offset) {
	DxLib::DrawBox((Left()+offset.x)*2, (Top()+offset.y)*2, (Right()+offset.x)*2, (Bottom()+offset.y)*2, 0xffffffff, false);
}

void
Vector2_2::operator*=(float scale) {
	x *= scale;
	y *= scale;
}

Vector2_2
Vector2_2::operator*(float scale) {
	return Vector2_2{ x * scale,y * scale };
}

Vector2_2 operator+(const Vector2_2& va, const Vector2_2 vb) {
	return Vector2_2(va.x + vb.x, va.y + vb.y);
}

Vector2_2 operator-(const Vector2_2& va, const Vector2_2 vb){
	return Vector2_2(va.x - vb.x, va.y - vb.y);
}

float
Vector2_2::Magnitude()const {
	return hypot(x, y);
}

float Vector2_2::SQMagnitude() const
{
	return x * x + y * y;
}

void 
Vector2_2::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}

Vector2_2
Vector2_2::Normalized() {
	float mag = Magnitude();
	return Vector2_2(x / mag,	y /mag);
}

///内積を返す
float
Dot(const Vector2_2& va, const Vector2_2& vb) {
	return va.x*vb.x + va.y*vb.y;
}

///外積を返す
float
Cross(const Vector2_2& va, const Vector2_2& vb) {
	return va.x*vb.y - vb.x*va.y;
}

///内積演算子
float 
operator*(const Vector2_2& va, const Vector2_2& vb) {
	return Dot(va, vb);
}

///外積演算子
float 
operator%(const Vector2_2& va, const Vector2_2& vb) {
	return Cross(va, vb);
}

void 
Vector2_2::operator+=(const Vector2_2& v) {
	x += v.x;
	y += v.y;
}
void 
Vector2_2::operator-=(const Vector2_2& v) {
	x -= v.x;
	y -= v.y;

}