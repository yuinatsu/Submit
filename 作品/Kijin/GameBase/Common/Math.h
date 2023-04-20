#pragma once
#include <algorithm>
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"


// Vector2Tmp��x=0,y=0��\���萔
template<ValueC T>
constexpr Vector2Tmp<T> zeroVector2{ static_cast<T>(0),static_cast<T>(0) };

// Vector2Tmp�̏����x=0,y=-1��\���萔
template<ValueC T>
constexpr Vector2Tmp<T> upVector2{ static_cast<T>(0),static_cast<T>(-1) };

// Vector2Tmp�̉�����x=0,y=1��\���萔
template<ValueC T>
constexpr Vector2Tmp<T> downVector2{ static_cast<T>(0),static_cast<T>(1) };

// Vector2Tmp�̉E����x=1,y=0��\���萔
template<ValueC T>
constexpr Vector2Tmp<T> rightVector2{ static_cast<T>(1),static_cast<T>(0) };

// Vector2Tmp�̏����x=-1,y=0��\���萔
template<ValueC T>
constexpr Vector2Tmp<T> leftVector2{ static_cast<T>(-1),static_cast<T>(0) };

// Vector3Tmp��x=0,y=0,z=0��\���萔
template<ValueC T>
constexpr Vector3Tmp<T> zeroVector3{ static_cast<T>(0),static_cast<T>(0) ,static_cast<T>(0) };

// Vector3Tmp�̏������\���萔
template<ValueC T>
constexpr Vector3Tmp<T> upVector3{ static_cast<T>(0),static_cast<T>(1) ,static_cast<T>(0) };

// Vector3Tmp�̉�������\���萔
template<ValueC T>
constexpr Vector3Tmp<T> downVector3{ static_cast<T>(0),static_cast<T>(-1) ,static_cast<T>(0) };

// Vector3Tmp�̑O��������\���萔
template<ValueC T>
constexpr Vector3Tmp<T> forwardVector3{ static_cast<T>(0),static_cast<T>(0) ,static_cast<T>(1) };

// Vector3Tmp�̌��������\���萔
template<ValueC T>
constexpr Vector3Tmp<T> backVector3{ static_cast<T>(0),static_cast<T>(0) ,static_cast<T>(-1) };

// Vector3Tmp�̍�������\���萔
template<ValueC T>
constexpr Vector3Tmp<T> leftVector3{ static_cast<T>(-1),static_cast<T>(0) ,static_cast<T>(0) };

// Vector3Tmp�̉E������\���萔
template<ValueC T>
constexpr Vector3Tmp<T> rightVector3{ static_cast<T>(1),static_cast<T>(0) ,static_cast<T>(0) };

/// <summary>
/// ��悷��
/// </summary>
/// <param name="val"> ��悵�����l </param>
/// <returns> ��悵���l </returns>
template<ValueC T>
constexpr T Square(T val)
{
	return val * val;
}

/// <summary>
/// ����(Vector2�p)
/// </summary>
/// <param name="lVec"></param>
/// <param name="rVec"></param>
/// <returns></returns>
template<ValueC T>
constexpr T Dot(const Vector2Tmp<T>& lVec, const Vector2Tmp<T>& rVec)
{
	return lVec.x * rVec.x + lVec.y * rVec.y;
}

/// <summary>
/// ����(Vector3�p)
/// </summary>
/// <param name="lVec"></param>
/// <param name="rVec"></param>
/// <returns></returns>
template<ValueC T>
constexpr T Dot(const Vector3Tmp<T>& lVec, const Vector3Tmp<T>& rVec)
{
	return lVec.x * rVec.x + lVec.y * rVec.y + lVec.z + rVec.z;
}

/// <summary>
/// �O��(Vector2�p)
/// </summary>
/// <param name="lVec"></param>
/// <param name="rVec"></param>
/// <returns></returns>
template<ValueC T>
constexpr T Cross(const Vector2Tmp<T>& lVec, const Vector2Tmp<T>& rVec)
{
	return lVec.x * rVec.y - lVec.y * rVec.x;
}


template<ValueC T>
constexpr Vector3Tmp<T> Cross(const Vector3Tmp<T>& lVec, const Vector3Tmp<T>& rVec)
{
	return { std::fma(lVec.y, rVec.z,-(lVec.z * rVec.y)),std::fma(lVec.z , rVec.x,-(lVec.x * rVec.z)), std::fma(lVec.x, rVec.y, -(lVec.y * rVec.x)) };
}

// �~����
template<ValueC T>
constexpr T pi = static_cast<T>(3.1415926535897932384626433832795);

// �~����*2
template<ValueC T>
constexpr T pi2 = pi<T> * static_cast<T>(2.0);

/// <summary>
/// �x���@���ʓx�@�ɂ���
/// </summary>
/// <param name="deg"> �ϊ��������x���@�̒l </param>
/// <returns>  </returns>
template<ValueC T>
constexpr T Deg2Rad(const T& deg)
{
	return deg * (pi<T> / static_cast<T>(180));
}

/// <summary>
/// �ʓx�@��x���@�ɂ���
/// </summary>
/// <param name="rad"></param>
/// <returns></returns>
template<ValueC T>
constexpr T Rad2Deg(const T& rad)
{
	return rad * (static_cast<T>(180) / pi<T>);
}

/// <summary>
/// �p�x���w��͈͂ɐ��K������
/// </summary>
/// <param name="angle"> �ϊ�����l </param>
/// <param name="valMin"> �ŏ��l(�f�t�H���g=0) </param>
/// <param name="valMax"> �ő�l(�f�t�H���g=pi2) </param>
/// <returns></returns>
float GetNormalizedAngle(float angle, float valMin = 0.0f, float valMax = pi2<float>);

double GetNormalizedAngle(double angle, double valMin = 0.0f, double valMax = pi2<double>);

float DirNearAroundRad(float from, float to);

MATRIX ToMatrix(Quaternion& q);

Quaternion ToQuaternion(MATRIX& m);
Quaternion ToQuaternion(Vector3& e);

Vector3 ToEuler(MATRIX& m);
Vector3 ToEuler(Quaternion& q);

template<class T>
constexpr T RadIn2PI(const T& rad)
{
	T tmp = std::fmod(rad, pi<T> *static_cast<T>(2));
	if (tmp < static_cast<T>(0))
	{
		tmp += pi<T> *static_cast<T>(2);
	}
	return tmp;
}
	
/// <summary>
/// �N�����v����
/// </summary>
/// <param name="val"> �Ώۂ̒l </param>
/// <param name="minVal"> �ŏ��l(�f�t�H���g0) </param>
/// <param name="maxVal"> �ő�l(�f�t�H���g1) </param>
/// <returns></returns>
constexpr float Clamp(float val, float minVal = 0, float maxVal = 1)
{
	return std::clamp(val,minVal,maxVal);
}

/// <summary> Vector2��Vector2�̊Ԃ̊p�x�����߂� </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
template<ValueC T>
T GetAngle2Vector(const Vector2Tmp<T>& v1, const Vector2Tmp<T>& v2)
{
	auto tmpv1 = v1.Normalized();
	auto tmpv2 = v2.Normalized();
	auto dot = Dot(tmpv1, tmpv2);
	auto cross = Cross(tmpv1, tmpv2);
	auto angle = atan2(cross, dot);
	if (angle < 0)
	{
		angle += pi<float> *2.0f;
	}
	return angle;
}



/// <summary> a��b��t�Ő��`�⊮����֐� </summary>
/// <param name="a">  </param>
/// <param name="b"></param>
/// <param name="t">�@1.0f ����0.0f�̒l�@</param>
/// <returns></returns>
template<ValueC T>
constexpr T Learp(const T& a, const T& b, const T& t)
{
	return a +  ((b - a) * t);
}

/// <summary> �x�N�g��a�ƃx�N�g��b��t�Ő��`�⊮����֐� </summary>
/// <param name="a">  </param>
/// <param name="b"></param>
/// <param name="t"> 1.0f ����0.0f�̒l </param>
/// <returns></returns>
template<ValueC T>
constexpr Vector2Tmp<T> Learp(const Vector2Tmp<T>& a, const Vector2Tmp<T>& b, const T& t)
{
	return a + ((b - a) * t) ;
}

template<ValueC T>
constexpr Vector3Tmp<T> Learp(const Vector3Tmp<T>& a, const Vector3Tmp<T>& b, const T& t)
{
	return a + ((b - a) * t);
}


/// <summary> Vector2Tmp��Clamp���� </summary>
/// <param name="val"> Clamp�������l </param>
/// <param name="min"> �ŏ��l </param>
/// <param name="max"> �ő�l </param>
/// <returns> Clamp��̒l </returns>
template<ValueC T>
constexpr Vector2Tmp<T> Clamp(const Vector2Tmp<T>& val, const Vector2Tmp<T>& min, const Vector2Tmp<T>& max)
{
	return { std::clamp(val.x,min.x,max.x),std::clamp(val.y,min.y,max.y) };
}

// <summary> Vector3Tmp��Clamp���� </summary>
/// <param name="val"> Clamp�������l </param>
/// <param name="min"> �ŏ��l </param>
/// <param name="max"> �ő�l </param>
/// <returns> Clamp��̒l </returns>
template<ValueC T>
constexpr Vector3Tmp<T> Clamp(const Vector3Tmp<T>& val, const Vector3Tmp<T>& min, const Vector3Tmp<T>& max)
{
	return { std::clamp(val.x,min.x,max.x),std::clamp(val.y,min.y,max.y),std::clamp(val.z,min.z,max.z) };
}

