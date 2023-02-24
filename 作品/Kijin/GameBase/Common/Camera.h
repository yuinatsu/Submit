#pragma once
#include <DxLib.h>
#include "Vector3.h"
#include "../Application.h"
class ObjectManager;
class Controller;

// �J�����N���X
class Camera
{
public:
	Camera();

	// �������玎���p�̃J����---------------------------------------------------
	static constexpr float HEGHT = 200.0f;
	// �����_����J�����܂ł�XZ����
	static constexpr float TARGET_TO_CAMERA = 500.0f;
	// �����_����L�����N�^�܂ł�XZ����
	static constexpr float TARGET_TO_CHAR = 300.0f;
	// ��ʂ̒��S (1280,720)
	static constexpr int HALF_SIZE_X = 640;
	static constexpr int HALF_SIZE_Y = 360;

	// �J�����̈ړ�����ƒ����_�̐ݒ�
	void Update(Controller& controller);

	Vector3 GetAngle(void);

	/// <summary>
	/// �J�����̃Z�b�g�A�b�v������
	/// </summary>
	/// <param name=""></param>
	void SetUpScreen(void) const;

	/// <summary>
	/// �J�������w����W�̕��֌�����
	/// </summary>
	/// <param name="pos"> �����������W </param>
	void Look(const Vector3& pos);

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

	void SetPos(const Vector3& pos)
	{
		pPos_ = pos;
	}
private:
	// �J�����̈ړ�����̌v�Z
	void rotate(float* posX,float* posY, const float angle, const Vector3 mpos);

	// �J�����̈ړ�����
	void ControlCamera(Controller& controller);
	// �J�����̒����_�̐ݒ�
	void SetBefore(void);

	// �m�F�p�̃A���O��
	Vector3 angle_;

	// ������̍��W
	Vector3 pPos_;

	// �X��
	MATRIX rotation_;

	// �J�����̍��W
	Vector3 pos_;

	// �����_
	Vector3 target_;
	VECTOR vtarget_;

	Vector2 speed_;

	// �J�����̃��[�C���O
	float yaw;
	// �}�E�X�̍��W���Z�b�g
	Vector2I mousePos_;
};

