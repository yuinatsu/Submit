#pragma once
#include <DxLib.h>

template<class T>
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();
	ConstantBuffer& operator=(const T& val);
	void Update(void);
	const int GetHandle(void) const;
private:
	T* ptr_;
	int handle_;
};

template<class T>
inline ConstantBuffer<T>::ConstantBuffer()
{
	handle_ = CreateShaderConstantBuffer((sizeof(T) + 0x0f) & ~0x0f);
	ptr_ = reinterpret_cast<T*>(GetBufferShaderConstantBuffer(handle_));
}

template<class T>
inline ConstantBuffer<T>::~ConstantBuffer()
{
	DeleteShaderConstantBuffer(handle_);
	ptr_ = nullptr;
}

template<class T>
inline ConstantBuffer<T>& ConstantBuffer<T>::operator=(const T& val)
{
	*ptr_ = val;
	return *this;
}

template<class T>
inline void ConstantBuffer<T>::Update(void)
{
	UpdateShaderConstantBuffer(handle_);
}

template<class T>
inline const int ConstantBuffer<T>::GetHandle(void) const
{
	return handle_;
}
