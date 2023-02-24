#pragma once
template<class T> class Vector2Template							// <> が出てきたらほぼほぼ”テンプレート”
{
public:
	Vector2Template();							// コンストラクタ
	Vector2Template(T x, T y);					// 引数付きコンストラクト
	~Vector2Template();							// デストラクタ
	T x;					// class T を使用することでどんな型でも対応できる
	T y;


	// 代入演算子
	//-------------
	Vector2Template& operator = (const Vector2Template& vec);			// 代入のオペレーター


	// 添え字演算子
	//---------------
	int& operator [] (int no);							// 配列のオペレーター		配列の〇番目などの指定なので class T を使う必要はない（int型である必要がある）


	// 単項演算子
	//-------------
	Vector2Template& operator += (const Vector2Template& vec);			// Vector2Template += Vector2Template	(インストラクタされたVector2 += 引数のVector2)
	Vector2Template& operator -= (const Vector2Template& vec);			// Vector2Template -= Vector2Template	(インストラクタされたVector2 -= 引数のVector2)
	Vector2Template& operator *= (const Vector2Template& vec);			// Vector2Template *= Vector2Template	(インストラクタされたVector2 *= 引数のVector2)
	Vector2Template& operator /= (const Vector2Template& vec);			// Vector2Template /= Vector2Template	(インストラクタされたVector2 /= 引数のVector2)

	Vector2Template& operator += (T num);						// Vector2Template += T		(インストラクタされたVector2 += 引数に対応した型)
	Vector2Template& operator -= (T num);						// Vector2Template -= T		(インストラクタされたVector2 -= 引数に対応した型)
	Vector2Template& operator *= (T num);						// Vector2Template *= T		(インストラクタされたVector2 *= 引数に対応した型)
	Vector2Template& operator /= (T num);						// Vector2Template /= T		(インストラクタされたVector2 /= 引数に対応した型)

	Vector2Template operator + ()const;							// Vector2自身に”＋”を付与 
	Vector2Template operator - ()const;							// Vector2自身に”ー”を付与


	// 比較演算子
	//-------------
	bool operator == (const Vector2Template& vec)const;			// Vector2Template == Vector2Template
	bool operator != (const Vector2Template& vec)const;			// Vector2Template != Vector2Template
	//bool operator < (const Vector2Template& vec)const;		// Vector2Template < Vector2Template
	//bool operator <= (const Vector2Template& vec)const;		// Vector2Template <= Vector2Template
	//bool operator > (const Vector2Template& vec)const;		// Vector2Template > Vector2Template
	//bool operator >= (const Vector2Template& vec)const;		// Vector2Template >= Vector2Template
};


// template<class T> ... class T を取得

template<class T> Vector2Template<T> operator + (const Vector2Template<T>& u, T v);			// Vector2Template + T
template<class T> Vector2Template<T> operator - (const Vector2Template<T>& u, T v);			// Vector2Template - T
template<class T> Vector2Template<T> operator * (const Vector2Template<T>& u, T v);			// Vector2Template * T
template<class T> Vector2Template<T> operator / (const Vector2Template<T>& u, T v);			// Vector2Template / T
template<class T> Vector2Template<T> operator % (const Vector2Template<T>& u, int v);		// Vector2Template % int			浮動小数点は % では使えない（小数点切り捨てになる）

template<class T> Vector2Template<T> operator + (const Vector2Template<T>& u, const Vector2Template<T>& v);			// Vector2Template + Vector2Template
template<class T> Vector2Template<T> operator - (const Vector2Template<T>& u, const Vector2Template<T>& v);			// Vector2Template - Vector2Template
template<class T> Vector2Template<T> operator * (const Vector2Template<T>& u, const Vector2Template<T>& v);			// Vector2Template * Vector2Template
template<class T> Vector2Template<T> operator / (const Vector2Template<T>& u, const Vector2Template<T>& v);			// Vector2Template / Vector2Template
template<class T> Vector2Template<T> operator % (const Vector2Template<T>& u, const Vector2Template<T>& v);			// Vector2Template / Vector2Template


using Vector2 = Vector2Template<int>;					// int型
using Vector2Dbl = Vector2Template<double>;				// double型
using Vector2Flt = Vector2Template<float>;				// float型


#include "details/Vector2.h"		// 定義を終えてから呼び出さないといけない
