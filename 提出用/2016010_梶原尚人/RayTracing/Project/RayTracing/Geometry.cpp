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


///���ς�Ԃ�
float
Dot(const Vector2& va, const Vector2& vb) {
	return va.x*vb.x + va.y*vb.y;
}

///�O�ς�Ԃ�
float
Cross(const Vector2& va, const Vector2& vb) {
	return va.x*vb.y - vb.x*va.y;
}

///���ω��Z�q
float 
operator*(const Vector2& va, const Vector2& vb) {
	return Dot(va, vb);
}

///�O�ω��Z�q
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


//-------��������3D�̃^�[��------
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


///���ς�Ԃ�
float
Dot(const Vector3& va, const Vector3& vb) {
	return va.x*vb.x + va.y*vb.y+va.z*vb.z;
}

///�O�ς�Ԃ�
Vector3
Cross(const Vector3& va, const Vector3& vb) {
	return Vector3(va.z*vb.y-va.y*vb.z,va.z*vb.x-va.x*vb.z,va.x*vb.y - vb.x*va.y);
}

///���ω��Z�q
float
operator*(const Vector3& va, const Vector3& vb) {
	return Dot(va, vb);
}

///�O�ω��Z�q
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
	//���C�����K���ς݂ł���O��Łc
	//���_���狅�̒��S�ւ̃x�N�g��(����)�����܂�
	auto center = pos - ray.start;
	//���S���王���ւ̓��ς��Ƃ�܂������x�N�g����
	auto d = Dot(center, ray.vec);
	//�����x�N�g���ƃx�N�g�����������āA���S����̐����������_�����߂܂�
	auto rdash = ray.vec * d;

	auto v = rdash - center;
	// d�Ɣ��a�Ɛ����̑傫�������_�܂ł̋���t�����߂ĎQ��t�ɑ��
	auto vLen2 = Dot(v, v);// ���g����ςƂ���Ƒ傫����2��ɂȂ�
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
	auto k = Dot(-ray.vec, N);				// 1�񂠂���̗������
	// �n�_�ƕ��ʂ̍ŒZ����
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
