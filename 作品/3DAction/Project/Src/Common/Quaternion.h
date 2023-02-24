#pragma once
#include <DxLib.h>
#include <iostream>
#include <algorithm>
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
	static Quaternion LookRotation(VECTOR dir, VECTOR up);

	// �s�񂩂�N�H�[�^�j�I���ɕϊ�
	static Quaternion GetRotation(MATRIX mat);

	// ��{�x�N�g�����擾
	VECTOR GetForward(void);
	VECTOR GetBack(void);
	VECTOR GetRight(void);
	VECTOR GetLeft(void);
	VECTOR GetUp(void);
	VECTOR GetDown(void);

	static double Dot(Quaternion q1, Quaternion q2);
	double Dot(Quaternion b);

	// ���K��
	Quaternion Normalized(void);
	void Normalize(void);

	// �t�N�H�[�^�j�I��
	Quaternion Inverse(void);

	// ���ʕ��
	static Quaternion Slerp(Quaternion from, Quaternion to, double t);


	// �Q�̃x�N�g���Ԃ̉�]�ʂ��擾����
	static Quaternion FromToRotation(VECTOR fromDir, VECTOR toDir);
	static Quaternion RotateTowards(Quaternion from, Quaternion to, float maxDegreesDelta);
	static double Angle(Quaternion q1, Quaternion q2);
	static Quaternion SlerpUnclamped(Quaternion& a, Quaternion& b, float t);
	static Quaternion Identity(void);

	// �Ώە����̉�]
	double Length(void);
	double LengthSquared(void);
	VECTOR xyz(void);
	static Quaternion Normalize(Quaternion q);
	static void Normalize(Quaternion q, Quaternion* result);

	void ToAngleAxis(float* angle, VECTOR* axis);


	//Quaternion operator*(float& rhs);
	//const Quaternion operator*(const float& rhs);
	//Quaternion operator+(Quaternion& rhs);
	//const Quaternion operator+(const Quaternion& rhs);

	//Quaternion operator*(Quaternion rhs);
	//VECTOR operator*(VECTOR rhs);

private:

	// ��{�x�N�g�����擾
	VECTOR GetDir(VECTOR dir);

	Quaternion operator*(float& rhs);
	const Quaternion operator*(const float& rhs);
	Quaternion operator+(Quaternion& rhs);
	const Quaternion operator+(const Quaternion& rhs);
	
};
