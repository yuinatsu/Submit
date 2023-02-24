#pragma once
template<class T> class Vector2Template
{
public:
	Vector2Template();
	Vector2Template(T x, T y);
	~Vector2Template();

	T x;
	T y;

	// “ñ€‰‰Zq
	Vector2Template operator + (const Vector2Template& vec);
	Vector2Template operator - (const Vector2Template& vec);
	Vector2Template operator * (const Vector2Template& vec);
	Vector2Template operator / (const Vector2Template& vec);
	Vector2Template operator % (const Vector2Template& vec);

	// ‘ã“ü‰‰Zq
	Vector2Template& operator = (const Vector2Template& vec);

	// “Y‚¦š‰‰Zq
	int& operator [] (int no);

	// ’P€‰‰Zq
	Vector2Template& operator += (const Vector2Template& vec);
	Vector2Template& operator -= (const Vector2Template& vec);
	Vector2Template& operator *= (const Vector2Template& vec);
	Vector2Template& operator /= (const Vector2Template& vec);
	Vector2Template& operator %= (const Vector2Template& vec);

	// •„†‚ğ•t—^
	Vector2Template operator + ()const;
	Vector2Template operator - ()const;

	// ”äŠr‰‰Zq
	bool operator == (const Vector2Template& vec)const;
	bool operator != (const Vector2Template& vec)const;
	bool operator < (const Vector2Template& vec)const;
	bool operator <= (const Vector2Template& vec)const;
	bool operator > (const Vector2Template& vec)const;
	bool operator >= (const Vector2Template& vec)const;
};

// “ñ€‰‰Zqi•Ê‚ÌŒ^“¯mj
template<class T> Vector2Template<T> operator + (const Vector2Template<T>& u, T v);
template<class T> Vector2Template<T> operator - (const Vector2Template<T>& u, T v);
template<class T> Vector2Template<T> operator * (const Vector2Template<T>& u, T v);
template<class T> Vector2Template<T> operator / (const Vector2Template<T>& u, T v);

template<class T> Vector2Template<T> operator + (T u, const Vector2Template<T>& v);
template<class T> Vector2Template<T> operator - (T u, const Vector2Template<T>& v);
template<class T> Vector2Template<T> operator * (T u, const Vector2Template<T>& v);
template<class T> Vector2Template<T> operator / (T u, const Vector2Template<T>& v);


using Vector2 = Vector2Template<int>;					// intŒ^
using Vector2Dbl = Vector2Template<double>;				// doubleŒ^
using Vector2Flt = Vector2Template<float>;				// floatŒ^

#include "detail/Vector2.h"
