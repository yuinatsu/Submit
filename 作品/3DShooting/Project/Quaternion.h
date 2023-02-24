#pragma once
#include "DxLib.h"

class Quaternion
{

public:
	
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	double w;
	double x;
	double y;
	double z;

	Quaternion(void);
	Quaternion(VECTOR rad);
	Quaternion(double w, double x, double y, double z);

	// �I�C���[�p����N�H�[�^�j�I���֕ϊ�
	static Quaternion Euler(VECTOR rad);
	static Quaternion Euler(double radX, double radY, double radZ);

	// �N�H�[�^�j�I���̍���
	static Quaternion Mult(Quaternion q1, Quaternion q2);
	Quaternion Mult(Quaternion q);

	// �w�莲���w��p���A��]������
	static Quaternion AngleAxis(double rad, VECTOR axis);

	// ���W����]������
	static VECTOR PosAxis(Quaternion q, VECTOR pos);
	VECTOR PosAxis(VECTOR pos);

	// �N�H�[�^�j�I������I�C���[�p�֕ϊ�
	static VECTOR ToEuler(Quaternion q);
	VECTOR ToEuler(void);

	// �N�H�[�^�j�I������s��֕ϊ�
	static MATRIX ToMatrix(Quaternion q);
	MATRIX ToMatrix(void);

	// �x�N�g������N�H�[�^�j�I���ɕϊ�
	static Quaternion LookRotation(VECTOR dir);

	// �s�񂩂�N�H�[�^�j�I���ɕϊ�
	static Quaternion GetRotation(MATRIX mat);

	// ��{�x�N�g�����擾
	VECTOR GetForward(void);
	VECTOR GetBack(void);
	VECTOR GetRight(void);
	VECTOR GetLeft(void);
	VECTOR GetUp(void);
	VECTOR GetDown(void);

	static float Dot(Quaternion q1, Quaternion q2);
	float Dot(Quaternion b);

	// ���K��
	Quaternion Normalized(void);
	void Normalize(void);

	// �t�N�H�[�^�j�I��
	Quaternion Inverse(void);

	// ���ʕ��
	static Quaternion Slerp(Quaternion from, Quaternion to, double t);


private:

	// ��{�x�N�g�����擾
	VECTOR GetDir(VECTOR dir);

	Quaternion operator*(float& rhs);
	const Quaternion operator*(const float& rhs);
	Quaternion operator+(Quaternion& rhs);
	const Quaternion operator+(const Quaternion& rhs);

};
