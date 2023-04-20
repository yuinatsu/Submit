#pragma once
#include "../Common/Vector3.h"

// �f�B���N�V�������C�g
struct DirectionLight
{
	Vector3 dir;			// ���˕���
	float padding1;			// �l�ߕ�
	Vector3 color;			// RGB�J���[
	float padding2;			// �l�ߕ�
	Vector3 eyePos;			// ���_���W
};
// �|�C���g���C�g
struct PointLight
{
	Vector3 pos;			// ���W
	float padding1;			// �l�ߕ�
	Vector3 color;			// RGB�J���[
	float padding2;			// �l�ߕ�
	float inflRange;		// �e���͈�(m)
};
// �X�|�b�g���C�g
struct SpotLight
{
	Vector3 pos;			// ���W
	float padding1;			// �l�ߕ�
	Vector3 color;			// RGB�J���[
	float padding2;			// �l�ߕ�
	Vector3 dir;			// ���˕���
	float padding3;			// �l�ߕ�
	float angle;			// ���ˊp�x
	float inflRange;		// �e���͈�(m)
};