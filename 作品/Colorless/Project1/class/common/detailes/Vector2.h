#include "../Vector2.h"
#include <cmath>

template<class T>
const Vector2Template<T> Vector2Template<T>::ZERO = { 0,0 };

template <class T>
Vector2Template<T>::Vector2Template()
{
	x = 0;
	y = 0;
}

template <class T>
Vector2Template<T>::Vector2Template(T x_, T y_)
{
	x = x_;
	y = y_;
}

template<class T>
Vector2Template<T>::Vector2Template(std::string x_, std::string y_)
{
	x = static_cast<T>(std::atoi(x_.c_str()));
	y = static_cast<T>(std::atoi(y_.c_str()));
}

template <class T>
Vector2Template<T>::~Vector2Template()
{
}


template<class T>
Vector2Template<T>& Vector2Template<T>::operator=(const Vector2Template<int>& vec)
{
	x = vec.x;
	y = vec.y;
	return *this;
}
template<class T>
Vector2Template<T>& Vector2Template<T>::operator=(const Vector2Template<float>& vec)
{
	x = vec.x;
	y = vec.y;
	return *this;
}
template<class T>
Vector2Template<T>& Vector2Template<T>::operator=(const Vector2Template<double>& vec)
{
	x = vec.x;
	y = vec.y;
	return *this;
}



template <class T>
Vector2Template<T>& Vector2Template<T>::operator+=(const Vector2Template& vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
	// TODO: return ステートメントをここに挿入します
}

template <class T>
Vector2Template<T>& Vector2Template<T>::operator-=(const Vector2Template& vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
	// TODO: return ステートメントをここに挿入します
}

template <class T>
Vector2Template<T>& Vector2Template<T>::operator*=(const Vector2Template& vec)
{
	x *= vec.x;
	y *= vec.y;
	return *this;
	// TODO: return ステートメントをここに挿入します
}

template <class T>
Vector2Template<T>& Vector2Template<T>::operator/=(const Vector2Template& vec)
{
	!vec.x ? x = 1000000000 : x /= vec.x;
	!vec.y ? y = 1000000000 : y /= vec.y;
	return *this;
	// TODO: return ステートメントをここに挿入します
}

template <class T>
bool Vector2Template<T>::operator>=(const Vector2Template& vec)const
{
	return (x >= vec.x && y >= vec.y);
}

template <class T>
bool Vector2Template<T>::operator<=(const Vector2Template& vec) const
{
	return (x <= vec.x && y <= vec.y);
}

template <class T>
bool Vector2Template<T>::operator>(const Vector2Template& vec) const
{
	return (x > vec.x && y > vec.y);
}

template <class T>
bool Vector2Template<T>::operator<(const Vector2Template& vec) const
{
	return (x < vec.x && y < vec.y);
}

template <class T>
bool Vector2Template<T>::operator==(const Vector2Template& vec) const
{
	return (x == vec.x && y == vec.y);
}

template <class T>
bool Vector2Template<T>::operator!=(const Vector2Template& vec) const
{
	return (x != vec.x && y != vec.y);
}

template <class T>
bool Vector2Template<T>::operator&&(const Vector2Template& vec) const
{
	return ((x && vec.x) && (y && vec.y));
}

template <class T>
bool Vector2Template<T>::operator||(const Vector2Template& vec) const
{
	return ((x || vec.x) || (y || vec.y));
}

template <class T>
T& Vector2Template<T>::operator[](const int& num)
{
	if (!num)
	{
		return x;
	}
	else if (num == 1)
	{
		return y;
	}
	return x;
	// TODO: return ステートメントをここに挿入します
}

template <class T>
Vector2Template<T> Vector2Template<T>::operator+(const Vector2Template& vec)const
{
	return Vector2Template<T>(x + vec.x,y + vec.y);
}

template <class T>
Vector2Template<T> Vector2Template<T>::operator-(const Vector2Template& vec)const
{
	return Vector2Template<T>(x - vec.x,y - vec.y);
}

template <class T>
Vector2Template<T> Vector2Template<T>::operator*(const Vector2Template& vec)const
{
	return Vector2Template<T>(x * vec.x, y * vec.y);
}

template <class T>
Vector2Template<T> Vector2Template<T>::operator/(const Vector2Template& vec)const
{
	Vector2Template<T> tmp(0,0);
	!vec.x ? tmp.x = 1000000000 : tmp.x = x / vec.x;
	!vec.y ? tmp.y = 1000000000 : tmp.y = y / vec.y;

	return tmp;
}

template <class T>
Vector2Template<T> Vector2Template<T>::operator%(const Vector2Template& vec) const
{
	Vector2Template<T> tmp(0,0);
	!vec.x ? tmp.x = 1000000000 : tmp.x = x % vec.x;
	!vec.y ? tmp.y = 1000000000 : tmp.y = y % vec.y;
	return tmp;
}

template <class T>
Vector2Template<T> Vector2Template<T>::operator+()const
{
	return Vector2Template<T>(x,y);
	//return Vector2(x+tmp.x,y+tmp.y);
}

template <class T>
Vector2Template<T> Vector2Template<T>::operator-()const
{
	return Vector2Template<T>(-x,-y);
}

template <class T>
Vector2Template<T>& Vector2Template<T>::operator++()
{
	x++;
	y++;
	return *this;
}

template <class T>
Vector2Template<T> Vector2Template<T>::operator++(int)
{
	Vector2Template<T> tmp = *this;
	x++;
	y++;
	//++*this;
	return tmp;
	// TODO: return ステートメントをここに挿入します
}

template <class T>
Vector2Template<T>& Vector2Template<T>::operator--()
{
	x--;
	y--;
	return *this;
}

template <class T>
Vector2Template<T> Vector2Template<T>::operator--(int)
{
	Vector2Template<T> tmp = *this;
	x--;
	y--;
	return tmp;
	// TODO: return ステートメントをここに挿入します
}

template<class T>
float Vector2Template<T>::Magnitude() const
{
	return hypot(x, y);
}

template<class T>
void Vector2Template<T>::Normalize()
{
	float mag = Magnitude();
	if (x != 0) {
		x /= mag;
	}
	if (y != 0) {
		y /= mag;
	}
}

template<class T>
Vector2Template<T> Vector2Template<T>::Normalized() const
{
	Vector2Template<T> ret = { x,y };
	ret.Normalize();
	return ret;
}

template<class T>
double Vector2Template<T>::pow(void)
{
	double t = 1;
	if (y == 0) {
		return 1;
	}
	for (int i = 0; i < y; i++) {
		t = t * x;
	}
	return t;
}

template<class T>
float Dot(const Vector2Template<T>& va, const Vector2Template<T>& vb)
{
	return va.x * vb.x + va.y * vb.y;
}

template<class T>
float Cross(const Vector2Template<T>& va, const Vector2Template<T>& vb)
{
	return va.x * vb.y - vb.x * va.y;
}

template<class T>
float operator*(const Vector2Template<T>& va, const Vector2Template<T>& vb)
{
	return Dot(va, vb);
}

template<class T>
float operator%(const Vector2Template<T>& va, const Vector2Template<T>& vb)
{
	return Cross(va, vb);
}

template <class T>
Vector2Template<T> operator+(const Vector2Template<T>& vec, const int& num)
{
	return Vector2Template<T>(vec.x + num,vec.y + num);
}

template <class T>
Vector2Template<T> operator-(const Vector2Template<T>& vec, const int& num)
{
	return Vector2Template<T>(vec.x - num, vec.y - num);
}

template <class T>
Vector2Template<T> operator*(const Vector2Template<T>& vec, const int& num)
{
	return Vector2Template<T>(vec.x * num, vec.y * num);
}

template <class T>
Vector2Template<T> operator/(const Vector2Template<T>&vec, const int& num)
{
	if (!num)
	{
		return Vector2Template<T>(1000000000, 1000000000);
	}
	return Vector2Template<T>(vec.x / num, vec.y / num);
}

template <class T>
Vector2Template<T> operator+(const int& num, const Vector2Template<T>& vec)
{
	return Vector2Template<T>(num + vec.x,num + vec.y);
}

template <class T>
Vector2Template<T> operator-(const int& num, const Vector2Template<T>& vec)
{
	return Vector2Template<T>(num - vec.x, num - vec.y);
}

template <class T>
Vector2Template<T> operator*(const int& num, const Vector2Template<T>& vec)
{
	return Vector2Template<T>(num * vec.x,num * vec.y);
}

template <class T>
Vector2Template<T> operator/(const int& num, const Vector2Template<T>& vec)
{
	return Vector2Template<T>(num / vec.x,num/vec.y);
}

template <class T>
Vector2Template<T> operator+(const Vector2Template<T>& vec, const float& num)
{
	return Vector2Template<T>(vec.x + num,vec.y + num);
}

template <class T>
Vector2Template<T> operator-(const Vector2Template<T>& vec, const float& num)
{
	return Vector2Template<T>(vec.x - num, vec.y - num);
}

template <class T>
Vector2Template<T> operator*(const Vector2Template<T>& vec, const float& num)
{
	return Vector2Template<T>(vec.x * num, vec.y * num);
}

template <class T>
Vector2Template<T> operator/(const Vector2Template<T>& vec, const float& num)
{
	if (!num)
	{
		return Vector2Template<T>(1000000000, 1000000000);
	}
	return Vector2Template<T>(vec.x / num, vec.y / num);
}

template<class T>
Vector2Template<T> operator+(const float& num, const Vector2Template<T>& vec)
{
	return Vector2Template<T>(vec.x + num, vec.y + num);
}

template<class T>
Vector2Template<T> operator*(const float& num, const Vector2Template<T>& vec)
{
	return Vector2Template<T>(vec.x * num, vec.y * num);
}