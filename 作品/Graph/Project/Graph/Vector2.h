#pragma once
template<class T> class Vector2Template
{
public:
	Vector2Template();
	Vector2Template(T x, T y);
	~Vector2Template();

	T x;
	T y;

	// �񍀉��Z�q
	Vector2Template operator + (const Vector2Template& vec);
	Vector2Template operator - (const Vector2Template& vec);
	Vector2Template operator * (const Vector2Template& vec);
	Vector2Template operator / (const Vector2Template& vec);
	Vector2Template operator % (const Vector2Template& vec);

	// ������Z�q
	Vector2Template& operator = (const Vector2Template& vec);

	// �Y�������Z�q
	int& operator [] (int no);

	// �P�����Z�q
	Vector2Template& operator += (const Vector2Template& vec);
	Vector2Template& operator -= (const Vector2Template& vec);
	Vector2Template& operator *= (const Vector2Template& vec);
	Vector2Template& operator /= (const Vector2Template& vec);
	Vector2Template& operator %= (const Vector2Template& vec);

	// ������t�^
	Vector2Template operator + ()const;
	Vector2Template operator - ()const;

	// ��r���Z�q
	bool operator == (const Vector2Template& vec)const;
	bool operator != (const Vector2Template& vec)const;
	bool operator < (const Vector2Template& vec)const;
	bool operator <= (const Vector2Template& vec)const;
	bool operator > (const Vector2Template& vec)const;
	bool operator >= (const Vector2Template& vec)const;
};

// �񍀉��Z�q�i�ʂ̌^���m�j
template<class T> Vector2Template<T> operator + (const Vector2Template<T>& u, T v);
template<class T> Vector2Template<T> operator - (const Vector2Template<T>& u, T v);
template<class T> Vector2Template<T> operator * (const Vector2Template<T>& u, T v);
template<class T> Vector2Template<T> operator / (const Vector2Template<T>& u, T v);

template<class T> Vector2Template<T> operator + (T u, const Vector2Template<T>& v);
template<class T> Vector2Template<T> operator - (T u, const Vector2Template<T>& v);
template<class T> Vector2Template<T> operator * (T u, const Vector2Template<T>& v);
template<class T> Vector2Template<T> operator / (T u, const Vector2Template<T>& v);


using Vector2 = Vector2Template<int>;					// int�^
using Vector2Dbl = Vector2Template<double>;				// double�^
using Vector2Flt = Vector2Template<float>;				// float�^

#include "detail/Vector2.h"
