#pragma once
template<class T> class Vector2Template							// <> ���o�Ă�����قڂقځh�e���v���[�g�h
{
public:
	Vector2Template();							// �R���X�g���N�^
	Vector2Template(T x, T y);					// �����t���R���X�g���N�g
	~Vector2Template();							// �f�X�g���N�^
	T x;					// class T ���g�p���邱�Ƃłǂ�Ȍ^�ł��Ή��ł���
	T y;


	// ������Z�q
	//-------------
	Vector2Template& operator = (const Vector2Template& vec);			// ����̃I�y���[�^�[


	// �Y�������Z�q
	//---------------
	int& operator [] (int no);							// �z��̃I�y���[�^�[		�z��́Z�ԖڂȂǂ̎w��Ȃ̂� class T ���g���K�v�͂Ȃ��iint�^�ł���K�v������j


	// �P�����Z�q
	//-------------
	Vector2Template& operator += (const Vector2Template& vec);			// Vector2Template += Vector2Template	(�C���X�g���N�^���ꂽVector2 += ������Vector2)
	Vector2Template& operator -= (const Vector2Template& vec);			// Vector2Template -= Vector2Template	(�C���X�g���N�^���ꂽVector2 -= ������Vector2)
	Vector2Template& operator *= (const Vector2Template& vec);			// Vector2Template *= Vector2Template	(�C���X�g���N�^���ꂽVector2 *= ������Vector2)
	Vector2Template& operator /= (const Vector2Template& vec);			// Vector2Template /= Vector2Template	(�C���X�g���N�^���ꂽVector2 /= ������Vector2)

	Vector2Template& operator += (T num);						// Vector2Template += T		(�C���X�g���N�^���ꂽVector2 += �����ɑΉ������^)
	Vector2Template& operator -= (T num);						// Vector2Template -= T		(�C���X�g���N�^���ꂽVector2 -= �����ɑΉ������^)
	Vector2Template& operator *= (T num);						// Vector2Template *= T		(�C���X�g���N�^���ꂽVector2 *= �����ɑΉ������^)
	Vector2Template& operator /= (T num);						// Vector2Template /= T		(�C���X�g���N�^���ꂽVector2 /= �����ɑΉ������^)

	Vector2Template operator + ()const;							// Vector2���g�Ɂh�{�h��t�^ 
	Vector2Template operator - ()const;							// Vector2���g�Ɂh�[�h��t�^


	// ��r���Z�q
	//-------------
	bool operator == (const Vector2Template& vec)const;			// Vector2Template == Vector2Template
	bool operator != (const Vector2Template& vec)const;			// Vector2Template != Vector2Template
	//bool operator < (const Vector2Template& vec)const;		// Vector2Template < Vector2Template
	//bool operator <= (const Vector2Template& vec)const;		// Vector2Template <= Vector2Template
	//bool operator > (const Vector2Template& vec)const;		// Vector2Template > Vector2Template
	//bool operator >= (const Vector2Template& vec)const;		// Vector2Template >= Vector2Template
};


// template<class T> ... class T ���擾

template<class T> Vector2Template<T> operator + (const Vector2Template<T>& u, T v);			// Vector2Template + T
template<class T> Vector2Template<T> operator - (const Vector2Template<T>& u, T v);			// Vector2Template - T
template<class T> Vector2Template<T> operator * (const Vector2Template<T>& u, T v);			// Vector2Template * T
template<class T> Vector2Template<T> operator / (const Vector2Template<T>& u, T v);			// Vector2Template / T
template<class T> Vector2Template<T> operator % (const Vector2Template<T>& u, int v);		// Vector2Template % int			���������_�� % �ł͎g���Ȃ��i�����_�؂�̂ĂɂȂ�j

template<class T> Vector2Template<T> operator + (const Vector2Template<T>& u, const Vector2Template<T>& v);			// Vector2Template + Vector2Template
template<class T> Vector2Template<T> operator - (const Vector2Template<T>& u, const Vector2Template<T>& v);			// Vector2Template - Vector2Template
template<class T> Vector2Template<T> operator * (const Vector2Template<T>& u, const Vector2Template<T>& v);			// Vector2Template * Vector2Template
template<class T> Vector2Template<T> operator / (const Vector2Template<T>& u, const Vector2Template<T>& v);			// Vector2Template / Vector2Template
template<class T> Vector2Template<T> operator % (const Vector2Template<T>& u, const Vector2Template<T>& v);			// Vector2Template / Vector2Template


using Vector2 = Vector2Template<int>;					// int�^
using Vector2Dbl = Vector2Template<double>;				// double�^
using Vector2Flt = Vector2Template<float>;				// float�^


#include "details/Vector2.h"		// ��`���I���Ă���Ăяo���Ȃ��Ƃ����Ȃ�
