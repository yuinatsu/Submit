#include "Vector2.h"


template<class T> Vector2Template<T>::Vector2Template()
{
	x_ = 0;
	y_ = 0;
}


template<class T> Vector2Template<T>::Vector2Template(T x, T y)
{
	x_ = x;
	y_ = y;
	// ["x_"��"x"�ɉ��������ꍇ]
	// ���O��Ԃ��g�������@ Vector2::x = x;
	// this�|�C���^���g�������@ this->x = x;
}


template<class T> Vector2Template<T>::~Vector2Template()
{
}


// �񍀉��Z�q
template<class T> Vector2Template<T> Vector2Template<T>::operator+(const Vector2Template& vec)
{
	return Vector2Template<T>{ x_ + vec.x_, y_ + vec.y_ };
}

template<class T> Vector2Template<T> Vector2Template<T>::operator-(const Vector2Template& vec)
{
	return Vector2Template<T>{ x_ - vec.x_, y_ - vec.y_ };
}

template<class T> Vector2Template<T> Vector2Template<T>::operator*(const Vector2Template& vec)
{
	return Vector2Template<T>{ x_ * vec.x_, y_ * vec.y_ };
}

template<class T> Vector2Template<T> Vector2Template<T>::operator/(const Vector2Template& vec)
{
	return Vector2Template<T>{ x_ / vec.x_, y_ / vec.y_ };
}

template<class T> Vector2Template<T> Vector2Template<T>::operator%(const Vector2Template& vec)
{
	return Vector2Template<T>{ x_ % vec.x_, y_ % vec.y_ };
}


// ������Z�q
template<class T> Vector2Template<T>& Vector2Template<T>::operator=(const Vector2Template& vec)
{
	x_ = vec.x_;
	y_ = vec.y_;
	return (*this);
}


// �Y�������Z�q
template<class T> int& Vector2Template<T>::operator[](int no)
{
	if (no == 0)
	{
		return x_;
	}
	else if (no == 1)
	{
		return y_;
	}
	else
	{
		return x_;
	}
	// Switch���g�p��
	//switch (no)
	//{
	//case 0:
	//	return x_;
	//	//break;
	//case 1:
	//	return y_;
	//	//break;
	//default:
	//	return x_;
	//	//break;
	//}
}


// �P�����Z�q
template<class T> Vector2Template<T>& Vector2Template<T>::operator+=(const Vector2Template& vec)
{
	x_ += vec.x_;
	y_ += vec.y_;
	return (*this);
}

template<class T> Vector2Template<T>& Vector2Template<T>::operator-=(const Vector2Template& vec)
{
	x_ -= vec.x_;
	y_ -= vec.y_;
	return (*this);
}

template<class T> Vector2Template<T>& Vector2Template<T>::operator*=(const Vector2Template& vec)
{
	x_ *= vec.x_;
	y_ *= vec.y_;
	return (*this);
}

template<class T> Vector2Template<T>& Vector2Template<T>::operator/=(const Vector2Template& vec)
{
	if (vec.x_ )
	{
		x_ /= vec.x_;
	}
	else
	{
		x_ = 0;
	}
	if (vec.y_)
	{
		y_ /= vec.y_;
	}
	else
	{
		y_ = 0;
	}
	return (*this);
}

template<class T> Vector2Template<T>& Vector2Template<T>::operator%=(const Vector2Template& vec)
{
	if (vec.x_)
	{
		x_ %= vec.x_;
	}
	else
	{
		x_ = 0;
	}
	if (vec.y_)
	{
		y_ %= vec.y_;
	}
	else
	{
		y_ = 0;
	}
	return (*this);
}


// ������t�^
template<class T> Vector2Template<T> Vector2Template<T>::operator+() const
{
	return (*this);
}

template<class T> Vector2Template<T> Vector2Template<T>::operator-() const
{
	return Vector2Template<T>{ -x_,-y_ };
}


// ��r���Z�q
template<class T> bool Vector2Template<T>::operator==(const Vector2Template& vec) const
{
	return ((x_ == vec.x_) && (y_ == vec.y_));
}

template<class T> bool Vector2Template<T>::operator!=(const Vector2Template& vec) const
{
	return !((x_ == vec.x_) && (y_ == vec.y_));
}

template<class T> bool Vector2Template<T>::operator<(const Vector2Template& vec) const
{
	return ((x_ < vec.x_) && (y_ < vec.y_));
}

template<class T> bool Vector2Template<T>::operator<=(const Vector2Template& vec) const
{
	return ((x_ <= vec.x_) && (y_ <= vec.y_));
}

template<class T> bool Vector2Template<T>::operator>(const Vector2Template& vec) const
{
	return ((x_ > vec.x_) && (y_ > vec.y_));
}

template<class T> bool Vector2Template<T>::operator>=(const Vector2Template& vec) const
{
	return ((x_ >= vec.x_) && (y_ >= vec.y_));
}


// �񍀉��Z�q(�ʂ̌^���m)
template<class T> Vector2Template<T> operator+(const Vector2Template<T>& u, T v)
{
	return Vector2Template<T>{ u.x_ + v, u.y_ + v };
}

template<class T> Vector2Template<T> operator-(const Vector2Template<T>& u, T v)
{
	return Vector2Template<T>{ u.x_ - v, u.y_ - v };
}

template<class T> Vector2Template<T> operator*(const Vector2Template<T>& u, T v)
{
	return Vector2Template<T>{ u.x_ * v, u.y_ * v };
}

template<class T> Vector2Template<T> operator/(const Vector2Template<T>& u, T v)
{
	return Vector2Template<T>{ u.x_ / v, u.y_ / v };
}


template<class T> Vector2Template<T> operator+(T u, const Vector2Template<T>& v)
{
	return Vector2Template<T>{ u + v.x_ , u + v.y_ };
}

template<class T> Vector2Template<T> operator-(T u, const Vector2Template<T>& v)
{
	return Vector2Template<T>{ u - v.x_ , u - v.y_ };
}

template<class T> Vector2Template<T> operator*(T u, const Vector2Template<T>& v)
{
	return Vector2Template<T>{ u * v.x_ , u * v.y_ };
}

template<class T> Vector2Template<T> operator/(T u, const Vector2Template<T>& v)
{
	return Vector2Template<T>{ u / v.x_ , u / v.y_ };
}
