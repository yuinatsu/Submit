#include "../Vector2.h"

template<class T> Vector2Template<T>::Vector2Template()
{
	x = 0;
	y = 0;
}

template<class T> Vector2Template<T>::Vector2Template(T x, T y)
{
	this->x = x;
	this->y = y;
}

template<class T> Vector2Template<T>::~Vector2Template()
{
}

// “ñ€‰‰Zq
template<class T> Vector2Template<T> Vector2Template<T>::operator+(const Vector2Template& vec)
{
	return Vector2Template<T>{ x + vec.x, y + vec.y };
}

template<class T> Vector2Template<T> Vector2Template<T>::operator-(const Vector2Template& vec)
{
	return Vector2Template<T>{ x - vec.x, y - vec.y };
}

template<class T> Vector2Template<T> Vector2Template<T>::operator*(const Vector2Template& vec)
{
	return Vector2Template<T>{ x * vec.x, y * vec.y };
}

template<class T> Vector2Template<T> Vector2Template<T>::operator/(const Vector2Template& vec)
{
	return Vector2Template<T>{ x / vec.x, y / vec.y };
}

template<class T> Vector2Template<T> Vector2Template<T>::operator%(const Vector2Template& vec)
{
	return Vector2Template<T>{ x % vec.x, y % vec.y };
}

// ‘ã“ü‰‰Zq
template<class T> Vector2Template<T>& Vector2Template<T>::operator=(const Vector2Template& vec)
{
	x = vec.x;
	y = vec.y;
	return (*this);
}

// “Y‚¦š‰‰Zq
template<class T> int& Vector2Template<T>::operator[](int no)
{
	if (no == 0)
	{
		return x;
	}
	else if (no == 1)
	{
		return y;
	}
	else
	{
		return x;
	}
}

// ’P€‰‰Zq
template<class T> Vector2Template<T>& Vector2Template<T>::operator+=(const Vector2Template& vec)
{
	x += vec.x;
	y += vec.y;
	return (*this);
}

template<class T> Vector2Template<T>& Vector2Template<T>::operator-=(const Vector2Template& vec)
{
	x -= vec.x;
	y -= vec.y;
	return (*this);
}

template<class T> Vector2Template<T>& Vector2Template<T>::operator*=(const Vector2Template& vec)
{
	x *= vec.x;
	y *= vec.y;
	return (*this);
}

template<class T> Vector2Template<T>& Vector2Template<T>::operator/=(const Vector2Template& vec)
{
	if (vec.x )
	{
		x /= vec.x;
	}
	else
	{
		x = 0;
	}
	if (vec.y)
	{
		y /= vec.y;
	}
	else
	{
		y = 0;
	}
	return (*this);
}

template<class T> Vector2Template<T>& Vector2Template<T>::operator%=(const Vector2Template& vec)
{
	if (vec.x)
	{
		x %= vec.x;
	}
	else
	{
		x = 0;
	}
	if (vec.y)
	{
		y %= vec.y;
	}
	else
	{
		y = 0;
	}
	return (*this);
}

// •„†‚ğ•t—^
template<class T> Vector2Template<T> Vector2Template<T>::operator+() const
{
	return (*this);
}

template<class T> Vector2Template<T> Vector2Template<T>::operator-() const
{
	return Vector2Template<T>{ -x,-y };
}

// ”äŠr‰‰Zq
template<class T> bool Vector2Template<T>::operator==(const Vector2Template& vec) const
{
	return ((x == vec.x) && (y == vec.y));
}

template<class T> bool Vector2Template<T>::operator!=(const Vector2Template& vec) const
{
	return !((x == vec.x) && (y == vec.y));
}

template<class T> bool Vector2Template<T>::operator<(const Vector2Template& vec) const
{
	return ((x < vec.x) && (y < vec.y));
}

template<class T> bool Vector2Template<T>::operator<=(const Vector2Template& vec) const
{
	return ((x <= vec.x) && (y <= vec.y));
}

template<class T> bool Vector2Template<T>::operator>(const Vector2Template& vec) const
{
	return ((x > vec.x) && (y > vec.y));
}

template<class T> bool Vector2Template<T>::operator>=(const Vector2Template& vec) const
{
	return ((x >= vec.x) && (y >= vec.y));
}

// “ñ€‰‰Zq(•Ê‚ÌŒ^“¯m)
template<class T> Vector2Template<T> operator+(const Vector2Template<T>& u, T v)
{
	return Vector2Template<T>{ u.x + v, u.y + v };
}

template<class T> Vector2Template<T> operator-(const Vector2Template<T>& u, T v)
{
	return Vector2Template<T>{ u.x - v, u.y - v };
}

template<class T> Vector2Template<T> operator*(const Vector2Template<T>& u, T v)
{
	return Vector2Template<T>{ u.x * v, u.y * v };
}

template<class T> Vector2Template<T> operator/(const Vector2Template<T>& u, T v)
{
	return Vector2Template<T>{ u.x / v, u.y / v };
}

template<class T> Vector2Template<T> operator+(T u, const Vector2Template<T>& v)
{
	return Vector2Template<T>{ u + v.x , u + v.y };
}

template<class T> Vector2Template<T> operator-(T u, const Vector2Template<T>& v)
{
	return Vector2Template<T>{ u - v.x , u - v.y };
}

template<class T> Vector2Template<T> operator*(T u, const Vector2Template<T>& v)
{
	return Vector2Template<T>{ u * v.x , u * v.y };
}

template<class T> Vector2Template<T> operator/(T u, const Vector2Template<T>& v)
{
	return Vector2Template<T>{ u / v.x , u / v.y };
}
