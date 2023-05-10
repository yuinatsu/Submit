#pragma once
#include "Vector3.h"

// �N�H�[�^�j�I���N���X
class Quaternion
{
public:

	/// <summary>
	/// �f�t�H���g�R���X�g���N�^
	/// </summary>
	constexpr Quaternion() :
		w{1.0f}, x{0.0f}, y{0.0f}, z{0.0f}
	{
	}

	/// <summary>
	/// �v�f�̒l���w��ł���R���X�g���N�^
	/// </summary>
	/// <param name="inW"></param>
	/// <param name="inX"></param>
	/// <param name="inY"></param>
	/// <param name="inZ"></param>
	constexpr Quaternion(float inW, float inX, float inY, float inZ) :
		w{inW}, x{inX}, y{inY}, z{inZ}
	{

	}

	/// <summary>
	/// �R�s�[�R���X�g���N�^
	/// </summary>
	/// <param name="q"></param>
	constexpr Quaternion(const Quaternion& q) :
		w{q.w}, x{q.x}, y{q.y}, z{q.z}
	{

	}

	/// <summary>
	/// �I�C���[�p(x,y,z)�Ŏw��ł���R���X�g���N�^
	/// </summary>
	/// <param name="radX"> x���̊p�x(���W�A��) </param>
	/// <param name="radY"> y���̊p�x(���W�A��) </param>
	/// <param name="radZ"> z���̊p�x(���W�A��) </param>
	Quaternion(float radX, float radY, float radZ);

	Quaternion(float rad, const Vector3& axis);

	constexpr Quaternion& operator=(const Quaternion& q) &
	{
		w = q.w;
		x = q.x;
		y = q.y;
		z = q.z;
		return *this;
	}

	constexpr Quaternion operator-() const
	{
		return { -w, -x, -y,-z };
	}

	/// <summary>
	/// ���g�ɑ΂��ăN�H�[�^�j�I��q��]������
	/// </summary>
	/// <param name="q"></param>
	/// <returns></returns>
	Quaternion& operator*=(const Quaternion& q)&;

	/// <summary>
	/// �I�C���[�p�֕ϊ������l���擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	Vector3 ToEuler(void) const;

	/// <summary>
	/// ���]������
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	void Inverse(void) &;

	/// <summary>
	/// ���]�����l���擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns> ���]�������N�H�[�^�j�I�� </returns>
	Quaternion Inversed(void) const;

	/// <summary>
	/// ��]�s��ɕϊ�����
	/// </summary>
	/// <param name="outMatrix"> �o�͐�̉�]�s�� </param>
	void ToMatrix(MATRIX& outMatrix) const;

	float Magnitude(void) const&;

	constexpr float SqMagnitude(void) const&
	{
		return (w * w + x * x + y * y + z * z);
	}

	void Normalize(void);

	Quaternion Normalized(void) const&;

	float w, x, y, z;
};

/// <summary>
/// �N�H�[�^�j�I�����m�̊|���Z(qr��ql����]������)
/// </summary>
/// <param name="ql"> ���� </param>
/// <param name="qr"> �E�� </param>
/// <returns> ��]��̃N�H�[�^�j�I�� </returns>
Quaternion operator*(const Quaternion& ql, const Quaternion& qr);

/// <summary>
/// �x�N�g������]������
/// </summary>
/// <param name="ql"></param>
/// <param name="vec"></param>
/// <returns></returns>
Vector3 operator*(const Quaternion& ql, const Vector3& vec);


