#pragma once
#include <algorithm>
#include "Vector2.h"

namespace Math
{
	// Vector2Tmp��x=0,y=0��\���萔
	template<Vec2C T>
	constexpr Vector2Tmp<T> zeroVector2{ static_cast<T>(0),static_cast<T>(0) };

	// Vector2Tmp�̏����x=0,y=-1��\���萔
	template<Vec2C T>
	constexpr Vector2Tmp<T> upVector2{ static_cast<T>(0),static_cast<T>(-1) };

	// Vector2Tmp�̉�����x=0,y=1��\���萔
	template<Vec2C T>
	constexpr Vector2Tmp<T> downVector2{ static_cast<T>(0),static_cast<T>(1) };

	// Vector2Tmp�̉E����x=1,y=0��\���萔
	template<Vec2C T>
	constexpr Vector2Tmp<T> rightVector2{ static_cast<T>(1),static_cast<T>(0) };

	// Vector2Tmp�̏����x=-1,y=0��\���萔
	template<Vec2C T>
	constexpr Vector2Tmp<T> leftVector2{ static_cast<T>(-1),static_cast<T>(0) };

	/// <summary>
	/// ��悷��
	/// </summary>
	/// <param name="val"> ��悵�����l </param>
	/// <returns> ��悵���l </returns>
	template<Vec2C T>
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
	template<Vec2C T>
	constexpr T Dot(const Vector2Tmp<T>& lVec, const Vector2Tmp<T>& rVec)
	{
		return lVec.x * rVec.x + lVec.y * rVec.y;
	}


	/// <summary>
	/// �O��(Vector2�p)
	/// </summary>
	/// <param name="lVec"></param>
	/// <param name="rVec"></param>
	/// <returns></returns>
	template<Vec2C T>
	constexpr T Cross(const Vector2Tmp<T>& lVec, const Vector2Tmp<T>& rVec)
	{
		return lVec.x * rVec.y - lVec.y * rVec.x;
	}


	// �~����
	template<Vec2C T>
	constexpr T pi = static_cast<T>(3.1415926535897932384626433832795);

	// �~����*2
	template<Vec2C T>
	constexpr T pi2 = pi<T> * static_cast<T>(2.0);

	/// <summary>
	/// �x���@���ʓx�@�ɂ���
	/// </summary>
	/// <param name="deg"> �ϊ��������x���@�̒l </param>
	/// <returns>  </returns>
	template<Vec2C T>
	constexpr T Deg2Rad(const T& deg)
	{
		return deg * (pi<T> / static_cast<T>(180));
	}

	/// <summary>
	/// �ʓx�@��x���@�ɂ���
	/// </summary>
	/// <param name="rad"></param>
	/// <returns></returns>
	template<Vec2C T>
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
	
	//
	constexpr float Clamp(float val, float minVal, float maxVal)
	{
		return std::clamp(val,minVal,maxVal);
	}

	/// <summary> Vector2��Vector2�̊Ԃ̊p�x�����߂� </summary>
	/// <param name="v1"></param>
	/// <param name="v2"></param>
	/// <returns></returns>
	template<Vec2C T>
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

	struct Matrix
	{
		float m[3][3];
	};

	// �P�ʍs��
	Matrix IdentityMat();

	/// <summary>
	/// ���s�ړ��s���Ԃ�
	/// </summary>
	/// <param name="x">X�������s�ړ���</param>
	/// <param name="y">Y�������s�ړ���</param>
	/// <returns></returns>
	Matrix TranslateMat(float x, float y);

	/// <summary>
	/// ��]�s���Ԃ�
	/// </summary>
	/// <param name="angle">��]�p�x</param>
	/// <returns></returns>
	Matrix RotateMat(float angle);

	/// <summary>
	/// 2�̍s�����Z����
	/// </summary>
	/// <param name="lmat">���Ӓl(�s��)</param>
	/// <param name="rmat">�E�Ӓl(�s��)</param>
	/// <returns></returns>
	Matrix MultipleMat(const Matrix& lmat, const Matrix& rmat);

	/// <summary>
	/// �x�N�g���ɑ΂��čs���Z��K�p���A���ʂ̃x�N�g����Ԃ�
	/// </summary>
	/// <param name="mat">�s��</param>
	/// <param name="vec">�x�N�g��</param>
	/// <returns></returns>
	Math::Vector2 MultipleVec(const Matrix& mat, const Math::Vector2& vec);


	Matrix operator*(const Matrix& lmat, const Matrix& rmat);
	Math::Vector2 operator*(const Matrix& mat, const Math::Vector2& vec);

	/// <summary> a��b��t�Ő��`�⊮����֐� </summary>
	/// <param name="a">  </param>
	/// <param name="b"></param>
	/// <param name="t">�@1.0f ����0.0f�̒l�@</param>
	/// <returns></returns>
	template<Vec2C T>
	constexpr T Learp(const T& a, const T& b, const T& t)
	{
		return a + ((b - a) * t);
	}

	/// <summary> �x�N�g��a�ƃx�N�g��b��t�Ő��`�⊮����֐� </summary>
	/// <param name="a">  </param>
	/// <param name="b"></param>
	/// <param name="t"> 1.0f ����0.0f�̒l </param>
	/// <returns></returns>
	template<Vec2C T>
	constexpr Vector2Tmp<T> Learp(const Vector2Tmp<T>& a, const Vector2Tmp<T>& b, const T& t)
	{
		return a + ((b - a) * t) ;
	}
	
	/// <summary> Vector2Tmp��Clamp���� </summary>
	/// <param name="val"> Clamp�������l </param>
	/// <param name="min"> �ŏ��l </param>
	/// <param name="max"> �ő�l </param>
	/// <returns> Clamp��̒l </returns>
	template<Vec2C T>
	constexpr Vector2Tmp<T> Clamp(const Vector2Tmp<T>& val, const Vector2Tmp<T>& min, const Vector2Tmp<T>& max)
	{
		return { std::clamp(val.x,min.x,max.x),std::clamp(val.y,min.y,max.y) };
	}

}

