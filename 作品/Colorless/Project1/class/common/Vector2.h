#pragma once
#include <vector>
#include <string>

#define GRAVITY 10.0f

template <class T>
class Vector2Template
{
public:
	Vector2Template();
	~Vector2Template();
	Vector2Template(T x_, T y_);

	Vector2Template(std::string x_,std::string y_);
	operator Vector2Template<int>() 
	{
		return Vector2Template<int>(static_cast<int>(x), static_cast<int>(y));
	};
	operator Vector2Template<float>() 
	{
		return Vector2Template<float>(static_cast<float>(x), static_cast<float>(y));
	};
	operator Vector2Template<double>() 
	{
		return Vector2Template<double>(static_cast<double>(x), static_cast<double>(y));
	};

	T x;
	T y;

	// ‘ã“ü‰‰Zq
	Vector2Template& operator = (const Vector2Template<int>& vec);
	Vector2Template& operator = (const Vector2Template<float>& vec);
	Vector2Template& operator = (const Vector2Template<double>& vec);

	// ’P€‰‰Zq
	Vector2Template& operator += (const Vector2Template& vec);
	Vector2Template& operator -= (const Vector2Template& vec);
	Vector2Template& operator *= (const Vector2Template& vec);
	Vector2Template& operator /= (const Vector2Template& vec);

	// ”äŠr‰‰Zq
	bool operator >= (const Vector2Template& vec)const;
	bool operator <=  (const Vector2Template& vec)const;
	bool operator > (const Vector2Template& vec)const;
	bool operator <  (const Vector2Template& vec)const;
	bool operator ==  (const Vector2Template& vec)const;
	bool operator != (const Vector2Template& vec)const;

	// ˜_—‰‰Zq 
	bool operator && (const Vector2Template& vec)const;
	bool operator || (const Vector2Template& vec)const;

	// “Y‚¦š‰‰Zq
	T& operator[](const int& num);

	// 2€‰‰Zq
	Vector2Template operator +(const Vector2Template& vec)const;
	Vector2Template operator -(const Vector2Template& vec)const;
	Vector2Template operator *(const Vector2Template& vec)const;
	Vector2Template operator /(const Vector2Template& vec)const;
	Vector2Template operator %(const Vector2Template& vec)const;

	Vector2Template operator +()const;
	Vector2Template operator -()const;

	Vector2Template& operator++();
	Vector2Template operator++(int);

	Vector2Template& operator--();
	Vector2Template operator--(int);

	// •½•û˜a‚Ì•½•ûª‚ğ•Ô‚·
	float Magnitude()const;	
	// ³‹K‰»(‘å‚«‚³‚ğ‚P‚É)‚µ‚Ü‚·
	void Normalize();
	// ³‹K‰»‚µ‚½‚à‚Ì‚ğ•Ô‚·
	Vector2Template Normalized()const;

	double pow(void);

	static const Vector2Template<T> ZERO;
};

///“àÏ‚ğ•Ô‚·
template <class T>
float Dot(const Vector2Template<T>& va, const Vector2Template<T>& vb);

///ŠOÏ‚ğ•Ô‚·
template <class T>
float Cross(const Vector2Template<T>& va, const Vector2Template<T>& vb);

///“àÏ‰‰Zq
template <class T>
float operator*(const Vector2Template<T>& va, const Vector2Template<T>& vb);

///ŠOÏ‰‰Zq
template <class T>
float operator%(const Vector2Template<T>& va, const Vector2Template<T>& vb);

// Vector2 ‚Æ int‚Ìl‘¥‰‰Z
template <class T>
Vector2Template<T> operator +(const Vector2Template<T>& vec,const int& num);
template <class T>
Vector2Template<T> operator -(const Vector2Template<T>& vec, const int& num);
template <class T>
Vector2Template<T> operator *(const Vector2Template<T>& vec, const int& num);
template <class T>
Vector2Template<T> operator /(const Vector2Template<T>& vec, const int& num);

// int ‚Æ Vector2‚Ìl‘¥‰‰Z
template <class T>
Vector2Template<T> operator +(const int& num,const Vector2Template<T>& vec);
template <class T>
Vector2Template<T> operator -(const int& num, const Vector2Template<T>& vec);
template <class T>
Vector2Template<T> operator *(const int& num, const Vector2Template<T>& vec);
template <class T>
Vector2Template<T> operator /(const int& num, const Vector2Template<T>& vec);

// Vector2 ‚Æ float‚Ìl‘¥‰‰Z
template <class T>
Vector2Template<T> operator + (const Vector2Template<T>& vec,const float& num);
template <class T>
Vector2Template<T> operator - (const Vector2Template<T>& vec, const float& num);
template <class T>
Vector2Template<T> operator * (const Vector2Template<T>& vec,const float& num);
template <class T>
Vector2Template<T> operator / (const Vector2Template<T>& vec, const float& num);
// float ‚Æ Vector2‚Ìl‘¥‰‰Z
template <class T>
Vector2Template<T> operator + (const float& num,const Vector2Template<T>& vec);
template <class T>
Vector2Template<T> operator * (const float& num,const Vector2Template<T>& vec);

using Vector2 = Vector2Template<int>;
using Vector2F = Vector2Template<float>;
using Vector2D = Vector2Template<double>;

using VecInt = std::vector<int>;
#include "./detailes/Vector2.h"