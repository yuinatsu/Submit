#include <cassert>
#include <cmath>
#include <numeric>
#include <limits>
#include "..\Vector2.h"


template <class T>
Vector2Template<T>::Vector2Template()
{
	x = 0;
	y = 0;
}

template <class T>
Vector2Template<T>::Vector2Template(T x, T y)
{
	this->x = x;
	this->y = y;
}

template <class T>
Vector2Template<T>::~Vector2Template()
{
}
template <class T>
Vector2Template<T>&Vector2Template<T>::operator=(const Vector2Template<T>& vec)
{
	x = vec.x;
	y = vec.y;
	return *this;
}
template <class T>
T& Vector2Template<T>::operator[](int num)
{
	if (num == 0)
	{
		return x;
	}
	else if (num == 1)
	{
		return y;
	}
	else
	{
		// ‘z’è‚µ‚Ä‚¢‚È‚¢“Y‚¦š‚ª—ˆ‚½ê‡‚Æ‚è‚ ‚¦‚¸x‚ğ‚©‚¦‚·
		//assert(!"‘z’è‚µ‚Ä‚¢‚È‚¢“Y‚¦šFVector2Temple");
		return x;
	}
}

template <class T>
Vector2Template<T>& Vector2Template<T>::operator+=(const Vector2Template<T>& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

template <class T>
Vector2Template<T>& Vector2Template<T>::operator-=(const Vector2Template<T>& vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

template<class T>
Vector2Template<T>& Vector2Template<T>::operator*=(const Vector2Template& vec)
{
	x *= vec.x;
	y *= vec.y;
	return *this;
}

template<class T>
Vector2Template<T>& Vector2Template<T>::operator/=(const Vector2Template& vec)
{
	if (vec.x == 0)
	{
		//assert(!"0œZ Œ^‚ÌÅ‘å’l‚ğ•Ô‚µ‚Ü‚·FVector2Temple");
		x = std::numeric_limits<T>::max();
	}
	else
	{
		x /= vec.x;
	}
	if (vec.y == 0)
	{
		//assert(!"0œZ Œ^‚ÌÅ‘å’l‚ğ•Ô‚µ‚Ü‚·FVector2Temple");
		y = std::numeric_limits<T>::max();
	}
	else
	{
		y /= vec.y;
	}
	return *this;
}

template <class T>
Vector2Template<T> Vector2Template<T>::operator+() const
{
	return *this;
}

template <class T>
Vector2Template<T> Vector2Template<T>::operator-() const
{
	return Vector2Template(-this->x, -this->y);
}


template <class T>
Vector2Template<T>& Vector2Template<T>::operator+=(const T u)
{
	this->x += u;
	this->y += u;
	return *this;
}
template<class T>
Vector2Template<T>& Vector2Template<T>::operator-=(const T u)
{
	this->x -= u;
	this->y -= u;
	return *this;
}
template <class T>
Vector2Template<T>& Vector2Template<T>::operator*=(const T k)
{
	x *= k;
	y *= k;
	return *this;
}

template <class T>
Vector2Template<T>& Vector2Template<T>::operator/=(const T k)
{
	if (k == 0)
	{
		//assert(!"0œZ Œ^‚ÌÅ‘å’l‚ğ•Ô‚µ‚Ü‚·FVector2Temple");
		x = std::numeric_limits<T>::max();
		y = std::numeric_limits<T>::max();
		return *this;
	}
	x /= k;
	y /= k;
	return *this;
}

template<class T>
Vector2Template<int> Vector2Template<T>::operator%=(const Vector2Template& vec)
{
	Vector2Template<int> intthis = static_cast<Vector2Template<int>>(*this);
	Vector2Template<int> intvec = static_cast<Vector2Template<int>>(vec);
	return Vector2Template<int>(intthis.x % intvec.x, intthis.y % intvec.y);
}

template <class T>
bool Vector2Template<T>::operator==(const Vector2Template<T>& vec) const
{
	return ((this->x == vec.x) && (this->y == vec.y));
}
template <class T>
bool Vector2Template<T>::operator!=(const Vector2Template<T>& vec) const
{
	return !((this->x == vec.x) && (this->y == vec.y));
}
template <class T>
bool Vector2Template<T>::operator<=(const Vector2Template<T>& vec) const
{
	return ((this->x <= vec.x) && (this->y <= vec.y));
}
template <class T>
bool Vector2Template<T>::operator<(const Vector2Template<T>& vec) const
{
	return ((this->x < vec.x) && (this->y < vec.y));
}
template <class T>
bool Vector2Template<T>::operator>=(const Vector2Template<T>& vec) const
{
	return ((this->x >= vec.x) && (this->y >= vec.y));
}
template <class T>
bool Vector2Template<T>::operator>(const Vector2Template<T>& vec) const
{
	return ((this->x > vec.x) && (this->y > vec.y));
}
template<class T>
inline T Vector2Template<T>::Magnitude() const
{
	return std::hypot(x, y);
}
template<class T>
inline void Vector2Template<T>::Nomalize()
{
	auto len = Magnitude();
	if (len == 0) return;

	x /= len;
	y /= len;
}
template<class T>
inline Vector2Template<T> Vector2Template<T>::Nomalized() const
{
	auto len = Magnitude();

	if (len == 0) return { 0,0 };

	Vector2Template<T> vec = { x / len,y / len };
	return vec;
}
template <class T>
Vector2Template<T> operator+(const Vector2Template<T>& vec, T u)
{
	return Vector2Template<T>(vec.x + u, vec.y + u);
}
template <class T>
Vector2Template<T> operator-(const Vector2Template<T>& vec, T u)
{
	return Vector2Template<T>(vec.x - u, vec.y - u);
}
template <class T>
Vector2Template<T> operator*(const Vector2Template<T>& vec, T u)
{
	return Vector2Template<T>(vec.x * u, vec.y * u);
}
template <class T>
Vector2Template<T> operator/(Vector2Template<T>& vec, T u)
{
	if (u == 0)
	{
		//assert(!"0œZ Œ^‚ÌÅ‘å’l‚ğ•Ô‚µ‚Ü‚·FVector2Temple");
		vec.x = std::numeric_limits<T>::max();
		vec.y = std::numeric_limits<T>::max();
		return vec;
	}
	return Vector2Template<T>(vec.x / u, vec.y / u);
}
template <class T>
Vector2Template<T> operator*(T u, const Vector2Template<T>& vec)
{
	return Vector2Template<T>(u * vec.x, u * vec.y);
}
template <class T>
Vector2Template<T> operator+(const Vector2Template<T>& veca, const Vector2Template<T>& vecb)
{
	return Vector2Template<T>(veca.x + vecb.x, veca.y + vecb.y);
}
template <class T>
Vector2Template<T> operator-(const Vector2Template<T>& veca, const Vector2Template<T>& vecb)
{
	return Vector2Template<T>(veca.x - vecb.x, veca.y - vecb.y);
}
template <class T>
Vector2Template<T> operator*(const Vector2Template<T>& veca, const Vector2Template<T>& vecb)
{
	return Vector2Template<T>(veca.x * vecb.x, veca.y * vecb.y);
}
template <class T>
Vector2Template<T> operator/(const Vector2Template<T>& veca, const Vector2Template<T>& vecb)
{
	if (vecb.x == 0 || vecb.y == 0)
	{
		//assert(!"0œZ Œ^‚ÌÅ‘å’l‚ğ•Ô‚µ‚Ü‚·FVector2Temple");
		return  Vector2Template<T>(std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
	}
	return Vector2Template<T>(veca.x / vecb.x, veca.y / vecb.y);
}
template <class T>
Vector2Template<int> operator%(const Vector2Template<T>& vec, const int u)
{
	Vector2Template<int> intvec = static_cast<Vector2Template<int>>(vec);
	return Vector2Template<int>(intvec.x % u, intvec.y % u);
}
template <class T>
Vector2Template<int> operator%(const Vector2Template<T>& veca, const Vector2Template<T>& vecb)
{
	Vector2Template<int> intveca = static_cast<Vector2Template<int>>(veca);
	Vector2Template<int> intvecb = static_cast<Vector2Template<int>>(vecb);
	return Vector2Template<int>(intveca.x % intvecb.x, intveca.y % intvecb.y);
}

template<class T>
inline T Dot(const Vector2Template<T>& veca, const Vector2Template<T>& vecb)
{
	return veca.x * vecb.x + veca.y * vecb.y;
}

template<class T>
inline T Cross(const Vector2Template<T>& veca, const Vector2Template<T>& vecb)
{
	return veca.x * vecb.y - veca.y * vecb.x;
}
