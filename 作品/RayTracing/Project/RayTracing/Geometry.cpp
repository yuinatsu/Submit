#include"Geometry.h"
#include<DxLib.h>
#include<cmath>


void
Rect::Draw() {
	DxLib::DrawBox(Left()*2, Top()*2, Right()*2, Bottom()*2, 0xffffffff, false);
}

void
Rect::Draw(Vector2& offset) {
	DxLib::DrawBox((Left()+offset.x)*2, (Top()+offset.y)*2, (Right()+offset.x)*2, (Bottom()+offset.y)*2, 0xffffffff, false);
}

void
Vector2::operator*=(float scale) {
	x *= scale;
	y *= scale;
}

Vector2
Vector2::operator*(float scale) {
	return Vector2(x*scale, y*scale);
}

Vector2 operator+(const Vector2& va, const Vector2 vb) {
	return Vector2(va.x + vb.x, va.y + vb.y);
}

Vector2 operator-(const Vector2& va, const Vector2 vb){
	return Vector2(va.x - vb.x, va.y - vb.y);
}

float
Vector2::Magnitude()const {
	return hypot(x, y);
}


void 
Vector2::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
}


Vector2
Vector2::Normalized() {
	float mag = Magnitude();
	return Vector2(x / mag,	y /mag);
}


///内積を返す
float
Dot(const Vector2& va, const Vector2& vb) {
	return va.x*vb.x + va.y*vb.y;
}

///外積を返す
float
Cross(const Vector2& va, const Vector2& vb) {
	return va.x*vb.y - vb.x*va.y;
}

///内積演算子
float 
operator*(const Vector2& va, const Vector2& vb) {
	return Dot(va, vb);
}

///外積演算子
float 
operator%(const Vector2& va, const Vector2& vb) {
	return Cross(va, vb);
}


void 
Vector2::operator+=(const Vector2& v) {
	x += v.x;
	y += v.y;
}
void 
Vector2::operator-=(const Vector2& v) {
	x -= v.x;
	y -= v.y;
}


//-------ここから3Dのターン------
void
Vector3::operator*=(float scale) {
	x *= scale;
	y *= scale;
	z *= scale;
}

Vector3
Vector3::operator*(float scale)const {
	return Vector3(x*scale, y*scale,z*scale);
}

Vector3 operator+(const Vector3& va, const Vector3 vb) {
	return Vector3(va.x + vb.x, va.y + vb.y,va.z+vb.z);
}

Vector3 operator-(const Vector3& va, const Vector3 vb) {
	return Vector3(va.x - vb.x, va.y - vb.y,va.z-vb.z);
}

float
Vector3::Magnitude()const {
	return sqrt(x*x+y*y+z*z);
}


void
Vector3::Normalize() {
	float mag = Magnitude();
	x /= mag;
	y /= mag;
	z /= mag;
}


Vector3
Vector3::Normalized() {
	float mag = Magnitude();
	return Vector3(x / mag, y / mag,z/mag);
}


///内積を返す
float
Dot(const Vector3& va, const Vector3& vb) {
	return va.x*vb.x + va.y*vb.y+va.z*vb.z;
}

///外積を返す
Vector3
Cross(const Vector3& va, const Vector3& vb) {
	return Vector3(va.z*vb.y-va.y*vb.z,va.z*vb.x-va.x*vb.z,va.x*vb.y - vb.x*va.y);
}

///内積演算子
float
operator*(const Vector3& va, const Vector3& vb) {
	return Dot(va, vb);
}

///外積演算子
Vector3
operator%(const Vector3& va, const Vector3& vb) {
	return Cross(va, vb);
}


void
Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}
void
Vector3::operator-=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}


bool
Sphere::IsHitRay(const Ray& ray, float& t, Vector3& N)const {
	//レイが正規化済みである前提で…
	//視点から球体中心へのベクトル(視線)を作ります
	auto center = pos - ray.start;
	//中心から視線への内積をとります＝＞ベクトル長
	auto d = Dot(center, ray.vec);
	//視線ベクトルとベクトル長をかけて、中心からの垂線下した点を求めます
	auto rdash = ray.vec * d;

	auto v = rdash - center;
	// dと半径と垂線の大きさから交点までの距離tを求めて参照tに代入
	auto vLen2 = Dot(v, v);// 自身を内積とすると大きさの2乗になる
	auto radius2 = radius * radius;
	auto w = sqrtf(radius2 - vLen2);
	t = d - w;
	if (t < 0.0f)
	{
		return false;
	}
	auto n = ray.vec * t - center;
	N = n.Normalized();

	return vLen2 <= radius2;
}

bool
Plane::IsHitRay(const Ray& ray, float& t, Vector3& refN)const {
	auto k = Dot(-ray.vec, N);				// 1回あたりの落ちる量
	// 始点と平面の最短距離
	auto h = Dot(ray.start, N) - offset;
	if (k <= 0.0f)
	{
		return false;
	}
	t = h / k;
	refN = N;
	return true;
}

Material::Material(const Color& col1, const Color& col2, float ambRate, float specRate, float specularPow, float refRate, Pattern pat) : color1(col1),color2(col2),ambientRate(ambRate),specularRate(specRate),specularity(specularPow),reflectionRate(refRate),pattern(pat)
{
}
