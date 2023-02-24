#pragma once

template<class T> class Vector2Template
{
public:
	Vector2Template();
	Vector2Template(T x, T y);
	~Vector2Template();
	T x;
	T y;

	// ������Z�q
	Vector2Template& operator = (const Vector2Template& vec);

	// �Y�������Z�q�i�z���[]�j�Ano�F�z��̉��Ԗڂ�
	int& operator [] (int no);

	// �P�����Z�q
	Vector2Template& operator +=(const Vector2Template& vec);
	Vector2Template& operator -=(const Vector2Template& vec);
	Vector2Template& operator *=(const Vector2Template& vec);
	Vector2Template& operator /=(const Vector2Template& vec);

	// const���ϐ��̑O�ɂ���ꍇ�F���̕ϐ��ɑ΂��ď������ݕs��
	// const����Ԍ��ɂ���ꍇ�F�S�Ă̎��ۂɂ����ď������ݕs��
	
	Vector2Template& operator +=(T vec);
	Vector2Template& operator -=(T vec);
	Vector2Template& operator *=(T vec);
	Vector2Template& operator /=(T vec);

	Vector2Template operator +() const;	// �������g�Ɂ{�L����t�^����
	Vector2Template operator -() const;	// �������g�Ɂ|�L����t�^����

	// ��r���Z�q
	bool operator == (const Vector2Template& vec) const;
	bool operator != (const Vector2Template& vec) const;
	bool operator <  (const Vector2Template& vec) const;
	bool operator <= (const Vector2Template& vec) const;
	bool operator >  (const Vector2Template& vec) const;
	bool operator >= (const Vector2Template& vec) const;
};
// �N���X�O�֐�
// Vector2 + T
template<class T> Vector2Template<T> operator+(const Vector2Template<T>& u, T v);

// Vector2 - T
template<class T> Vector2Template<T> operator-(const Vector2Template<T>& u, T v);

// Vector2 * T
template<class T> Vector2Template<T> operator*(const Vector2Template<T>& u, T v);

// Vector2 / T
template<class T> Vector2Template<T> operator/(const Vector2Template<T>& u, T v);

// Vector2 % int
template<class T> Vector2Template<T> operator%(const Vector2Template<T>& u, int v);

// Vector2 + Vector2
template<class T> Vector2Template<T> operator+(const Vector2Template<T>& u, const Vector2Template<T>& v);

// Vector2 - Vector2
template<class T> Vector2Template<T> operator-(const Vector2Template<T>& u, const Vector2Template<T>& v);

// Vector2 * Vector2
template<class T> Vector2Template<T> operator*(const Vector2Template<T>& u, const Vector2Template<T>& v);

// Vector2 / Vector2
template<class T> Vector2Template<T> operator/(const Vector2Template<T>& u, const Vector2Template<T>& v);

// Vector2 % Vector2
template<class T> Vector2Template<T> operator%(const Vector2Template<T>& u, const Vector2Template<T>& v);

using Vector2  = Vector2Template<int>;
using Vector2D = Vector2Template<double>;
using Vector2F = Vector2Template<float>;
#include "details/Vector2.h"
