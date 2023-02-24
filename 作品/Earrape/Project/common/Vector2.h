#pragma once

template<class T> class Vector2Template
{
public:
	Vector2Template();
	Vector2Template(T x, T y);
	~Vector2Template();
	T x;
	T y;

	// 代入演算子
	Vector2Template& operator = (const Vector2Template& vec);

	// 添え字演算子（配列の[]）、no：配列の何番目か
	int& operator [] (int no);

	// 単項演算子
	Vector2Template& operator +=(const Vector2Template& vec);
	Vector2Template& operator -=(const Vector2Template& vec);
	Vector2Template& operator *=(const Vector2Template& vec);
	Vector2Template& operator /=(const Vector2Template& vec);

	// constが変数の前にある場合：その変数に対して書き込み不可
	// constが一番後ろにある場合：全ての事象において書き込み不可
	
	Vector2Template& operator +=(T vec);
	Vector2Template& operator -=(T vec);
	Vector2Template& operator *=(T vec);
	Vector2Template& operator /=(T vec);

	Vector2Template operator +() const;	// 自分自身に＋記号を付与する
	Vector2Template operator -() const;	// 自分自身に－記号を付与する

	// 比較演算子
	bool operator == (const Vector2Template& vec) const;
	bool operator != (const Vector2Template& vec) const;
	bool operator <  (const Vector2Template& vec) const;
	bool operator <= (const Vector2Template& vec) const;
	bool operator >  (const Vector2Template& vec) const;
	bool operator >= (const Vector2Template& vec) const;
};
// クラス外関数
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
