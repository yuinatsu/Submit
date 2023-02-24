#pragma once
template <class T> class Vector2Template
{
public:
	Vector2Template();
	Vector2Template(T x, T y);
	~Vector2Template();
	T x;
	T y;
	
	//ë„ì¸ââéZéq
	Vector2Template& operator = (const Vector2Template& vec);
	
	// ìYÇ¶éöââéZéq
	T& operator [] (int num);
	
	// íPçÄââéZéq
	Vector2Template& operator += (const Vector2Template& vec);
	Vector2Template& operator -= (const Vector2Template& vec);
	Vector2Template& operator *= (const Vector2Template& vec);
	Vector2Template& operator /= (const Vector2Template& vec);

	Vector2Template operator + ()const;
	Vector2Template operator - ()const;

	Vector2Template& operator += (const T u);
	Vector2Template& operator -= (const T u);
	Vector2Template& operator *= (const T k);
	Vector2Template& operator /= (const T k);

	Vector2Template<int> operator %= (const Vector2Template& vec);

	// ÉLÉÉÉXÉgââéZéq
	explicit operator Vector2Template<int>()
	{
		return Vector2Template<int>(static_cast<int>(x), static_cast<int>(y));
	};

	explicit operator Vector2Template<float>()
	{
		return Vector2Template<float>(static_cast<float>(x), static_cast<float>(y));
	};

	explicit operator Vector2Template<double>()
	{
		return Vector2Template<double>(static_cast<double>(x), static_cast<double>(y));
	};

	// î‰ärââéZéq
	bool operator == (const Vector2Template& vec) const;
	bool operator != (const Vector2Template& vec) const;
	bool operator <= (const Vector2Template& vec) const;
	bool operator < (const Vector2Template& vec) const;
	bool operator >= (const Vector2Template& vec) const;
	bool operator > (const Vector2Template& vec) const;

	T Magnitude()const;
	void Nomalize();
	Vector2Template Nomalized()const;

};
template <class T>
Vector2Template<T> operator + (const Vector2Template<T>& vec, T u);
template <class T>
Vector2Template<T> operator - (const Vector2Template<T>& vec, T u);
template <class T>
Vector2Template<T> operator * (const Vector2Template<T>& vec, T u);
template <class T>
Vector2Template<T> operator / (Vector2Template<T>& vec, T u);
template <class T>
Vector2Template<T> operator * (T u, const Vector2Template<T>& vec);
template <class T>
Vector2Template<T> operator + (const Vector2Template<T>& veca, const Vector2Template<T>& vecb);
template <class T>
Vector2Template<T> operator - (const Vector2Template<T>& veca, const Vector2Template<T>& vecb);
template <class T>
Vector2Template<T> operator * (const Vector2Template<T>& veca, const Vector2Template<T>& vecb);
template <class T>
Vector2Template<T> operator / (const Vector2Template<T>& veca, const Vector2Template<T>& vecb);

template <class T>
Vector2Template<int> operator % (const Vector2Template<T>& vec, const int u);
template <class T>
Vector2Template<int> operator % (const Vector2Template<T>& veca, const Vector2Template<T>& vecb);

template <class T>
T Dot(const Vector2Template<T>& veca, const Vector2Template<T>& vecb);

template <class T>
T Cross(const Vector2Template<T>& veca, const Vector2Template<T>& vecb);

using Vector2 = Vector2Template<int>;
using Vector2Dbl = Vector2Template<double>;
using Vector2Flt = Vector2Template<float>;

using Size = Vector2Template<int>;

#include "details/Vector2.h"