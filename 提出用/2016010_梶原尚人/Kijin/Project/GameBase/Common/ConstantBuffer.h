#pragma once
#include <concepts>
#include <DxLib.h>
#include <array>
#include <cassert>

// �|�C���^������Ȃ��悤�ɃR���Z�v�g��
template<class T>
concept CBufC = std::is_class<T>::value;

// �萔�o�b�t�@�p�N���X
template<CBufC T>
class ConstantBuffer
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ConstantBuffer();

	~ConstantBuffer();

	/// <summary>
	/// �A�b�v�f�[�g
	/// </summary>
	/// <param name=""></param>
	void Update(void);


	/// <summary>
	/// �n���h���̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const int& GetHandle(void) const;

	// �|�C���^
	T* ptr_;
private:

	// �R�s�[���Z�q�ƃR�s�[�R���X�g���N�^���폜���Ƃ�
	ConstantBuffer(const ConstantBuffer&) = delete;
	ConstantBuffer& operator=(const ConstantBuffer&) = delete;

	// �n���h��
	int handle_;
};

template<CBufC T>
inline ConstantBuffer<T>::ConstantBuffer()
{
	constexpr int size = (sizeof(T) + 0x0f) & ~0x0f;
	handle_ = CreateShaderConstantBuffer(size);
	ptr_ = reinterpret_cast<T*>(GetBufferShaderConstantBuffer(handle_));
}

template<CBufC T>
inline ConstantBuffer<T>::~ConstantBuffer()
{
	DeleteShaderConstantBuffer(handle_);
	ptr_ = nullptr;
}

template<CBufC T>
inline void ConstantBuffer<T>::Update(void)
{
	UpdateShaderConstantBuffer(handle_);
}

template<CBufC T>
inline const int& ConstantBuffer<T>::GetHandle(void) const
{
	return handle_;
}

template<CBufC T, size_t size>
class ConstantBufferArray
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ConstantBufferArray();

	~ConstantBufferArray();

	/// <summary>
	/// �A�b�v�f�[�g
	/// </summary>
	/// <param name=""></param>
	void Update(void);


	/// <summary>
	/// �n���h���̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> �n���h��(int) </returns>
	const int& GetHandle(void) const;

	/// <summary>
	/// �z��ɃA�N�Z�X����
	/// </summary>
	/// <param name="s"> �A�N�Z�X����C���f�b�N�X </param>
	/// <returns></returns>
	T& operator[](size_t idx)&;

	/// <summary>
	/// �T�C�Y�̎擾
	/// </summary>
	/// <param name=""> �z��̃T�C�Y </param>
	/// <returns></returns>
	constexpr size_t Length(void) { return size; }

	
private:

	// �R�s�[���Z�q�ƃR�s�[�R���X�g���N�^���폜���Ƃ�
	ConstantBufferArray(const ConstantBuffer&) = delete;
	ConstantBufferArray& operator=(const ConstantBuffer&) = delete;

	std::array<T, size> array_;

	// �|�C���^
	T* ptr_;

	// �n���h��
	int handle_;
};

template<CBufC T, size_t size>
inline ConstantBufferArray<T, size>::ConstantBufferArray()
{
	constexpr auto bufSize = (sizeof(T) * size + 0x0f) & ~0x0f;
	handle_ = CreateShaderConstantBuffer(bufSize);
	ptr_ = reinterpret_cast<T*>(GetBufferShaderConstantBuffer(handle_));
}

template<CBufC T, size_t size>
inline ConstantBufferArray<T, size>::~ConstantBufferArray()
{
	DeleteShaderConstantBuffer(handle_);
	ptr_ = nullptr;
}

template<CBufC T, size_t size>
inline void ConstantBufferArray<T, size>::Update(void)
{
	std::copy(array_.begin(), array_.end(), ptr_);
}

template<CBufC T, size_t size>
inline const int& ConstantBufferArray<T, size>::GetHandle(void) const
{
	return handle_;
}

template<CBufC T, size_t size>
inline T& ConstantBufferArray<T, size>::operator[](size_t idx)&
{
	assert(size > idx);
	return array_[idx];
}
