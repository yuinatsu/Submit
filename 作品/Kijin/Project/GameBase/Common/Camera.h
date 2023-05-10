#pragma once
#include <DxLib.h>
#include "Vector3.h"
#include "ScreenID.h"
#include "../SceneManager.h"
#include "Quaternion.h"

class ObjectManager;
class Controller;

// �J�����N���X
class Camera
{
public:

	/// <summary>
	/// �`��悤�ɃJ�����̃Z�b�g�A�b�v������
	/// </summary>
	/// <param name=""></param>
	void SetUpScreen(void) const;

	/// <summary>
	/// �J�����̏�������擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns> ����� </returns>
	const Vector3 GetUp(void)const;

	/// <summary>
	/// �J�����̑O�������擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetForward(void) const;


	/// <summary>
	/// �J�����̍��W���Z�b�g����
	/// </summary>
	/// <param name="pos"></param>
	void SetPos(const Vector3& pos)
	{
		pos_ = pos;
	}

	/// <summary>
	/// �V���h�E�}�b�v�`��p�ɃJ�������Z�b�g����
	/// </summary>
	/// <param name="size"></param>
	/// <param name="n"> near </param>
	/// <param name="f"> fur </param>
	/// <param name="target"> �����_ </param>
	void SetUpShadow(float size, float n, float f, const Vector3& target) const;

	/// <summary>
	/// ��]���N�H�[�^�j�I���ŃZ�b�g����
	/// </summary>
	/// <param name="q"></param>
	void SetRotation(const Quaternion& q);
private:

	// �J�����̍��W
	Vector3 pos_;

	// �J�����̉�]
	Quaternion qRot_;

};

