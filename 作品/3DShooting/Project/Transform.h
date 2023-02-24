#pragma once
#include "DxLib.h"
#include "Quaternion.h"

/// <summary>
/// ���f������̊�{���
/// �傫���FVECTOR�
/// ��]�@�FQuaternion�
/// �ʒu�@�FVECTOR�
/// </summary>
class Transform
{

public:

	Transform(void);
	Transform(int model);

	// ���f���̃n���h��ID
	int modelId;

	// �傫��
	VECTOR scl;
	// ��]
	VECTOR rot;
	// �ʒu
	VECTOR pos;

	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	// ��]
	Quaternion quaRot;

	// ���[�J����]
	Quaternion quaRotLocal;

	/// <summary>
	/// ���f������̊�{���X�V
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	void SetModel(int model);

	// �O���������擾
	VECTOR GetForward(void);

	// ����������擾
	VECTOR GetBack(void);

	// �E�������擾
	VECTOR GetRight(void);

	// ���������擾
	VECTOR GetLeft(void);

	// ��������擾
	VECTOR GetUp(void);

	// ���������擾
	VECTOR GetDown(void);

	// �Ώە������擾
	VECTOR GetDir(VECTOR vec);

};

