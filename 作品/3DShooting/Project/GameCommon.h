#pragma once
#include "DxLib.h"
#include "Quaternion.h"

// ifdef�g���܂��傤�I
#define IS_DEBUG false

// �X�N���[���T�C�Y
#define SCREEN_SIZE_X 1024
#define SCREEN_SIZE_Y 640

// ����
enum class DIR
{
	DOWN,
	LEFT,
	RIGHT,
	UP,
	MAX
};


///// <summary>
///// ���f������̊�{���
///// �傫���FVECTOR�
///// ��]�@�FQuaternion�
///// �ʒu�@�FVECTOR�
///// </summary>
//struct Transform
//{
//
//	// �傫��
//	VECTOR scl;
//	// ��]
//	VECTOR rot;
//	// �ʒu
//	VECTOR pos;
//
//	MATRIX matScl;
//	MATRIX matRot;
//	MATRIX matPos;
//
//	// ��]
//	Quaternion quaRot;
//	// ���[�J����]
//	Quaternion quaRotLocal;
//
//};

