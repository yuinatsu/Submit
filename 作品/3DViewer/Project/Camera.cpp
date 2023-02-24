#include <dxlib.h>
#include "AsoUtility.h"
#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{
}

void Camera::Init(void)
{
	// �J�����̏����ݒ�
	pos_ = { 0.0f, HEIGHT, -DIS_TARGET_TO_CAMERA };

	// �J�����p�x(rad)
	angles_ = {
		//30.0f * (DX_PI_F / 180.0f), 
		AsoUtility::Deg2RadF(30.0f),
		0.0f,
		0.0f };
}

void Camera::Update(void)
{
	float movePow = 5.0f;
	float moveHPow = movePow / 2.0f;
	// �L�[�{�[�h����ŃJ�����ʒu���ړ�
	float rad = 0.0f;
	//// �c�ړ�--------------------
	//if (CheckHitKey(KEY_INPUT_W))
	//{
	//	// ���[���h���W�̑O��
	//	// Z������
	//	//pos_.z += movePow;
	//	// �J�����̑O��
	//	rad = AsoUtility::Deg2RadD(0.0f);
	//	pos_.x += sinf(angles_.y + rad) * moveHPow;
	//	pos_.z += cosf(angles_.y + rad) * moveHPow;
	//}
	//if (CheckHitKey(KEY_INPUT_S))
	//{
	//	// ���[���h���W�̌��
	//	// Z������
	//	//pos_.z -= movePow;
	//	// �J�����̌��
	//	rad = AsoUtility::Deg2RadD(0.0f);
	//	pos_.x -= sinf(angles_.y + rad) * moveHPow;
	//	pos_.z -= cosf(angles_.y + rad) * moveHPow;
	//}
	//// ���ړ�--------------------
	//if (CheckHitKey(KEY_INPUT_D))
	//{
	//	// ���[���h���W�̉E��
	//	// X������
	//	//pos_.x += movePow;
	//	// �J�����̉E����
	//	rad = AsoUtility::Deg2RadD(-90.0);
	//	pos_.x -= sinf(angles_.y + rad) * moveHPow;
	//	pos_.z -= cosf(angles_.y + rad) * moveHPow;
	//}
	//if (CheckHitKey(KEY_INPUT_A))
	//{
	//	// ���[���h���W�̍���
	//	// X������
	//	//pos_.x -= movePow;
	//	// �J�����̍�����
	//	rad = AsoUtility::Deg2RadD(-90.0);
	//	pos_.x += sinf(angles_.y + rad) * moveHPow;
	//	pos_.z += cosf(angles_.y + rad) * moveHPow;
	//}

	//// �����ړ�------------------
	//if (CheckHitKey(KEY_INPUT_N))
	//{
	//	// Y������
	//	pos_.y += movePow;
	//}
	//if (CheckHitKey(KEY_INPUT_M))
	//{
	//	// Y������
	//	pos_.y -= movePow;
	//}

	// �L�[�{�[�h����ŃJ�����p�x��ύX
	// ����]----------------------
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		// �E��]
		angles_.y += AsoUtility::Deg2RadD(1.0f);
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		// ����]
		angles_.y -= AsoUtility::Deg2RadD(1.0f);
	}
	//// �c��]-----------------------
	//if (CheckHitKey(KEY_INPUT_DOWN))
	//{
	//	// ���]
	//	angles_.x += AsoUtility::Deg2RadD(1.0f);
	//}
	//if (CheckHitKey(KEY_INPUT_UP))
	//{
	//	// ����]
	//	angles_.x -= AsoUtility::Deg2RadD(1.0f);
	//}
}

void Camera::SetBeforDraw(void)
{
	if (unit_ == nullptr)
	{
		// �J�������Z�b�g
		SetCameraPositionAndAngle(pos_, angles_.x, angles_.y, angles_.z);
	}
	else
	{
		float dirX;
		float dirZ;
		VECTOR dir;
		VECTOR movePow;

		// �J�����̌����Ă������
		dirX = sin(angles_.y);
		dirZ = cos(angles_.y);

		// ���K��
		dir = VNorm({ dirX, 0.0f, dirZ });

		// ��������ړ��ʂ��Z�o
		movePow = VScale(dir, DIS_TARGET_TO_UNIT);

		VECTOR uPos = unit_->GetPos();
		targetPos_ = VAdd(uPos, movePow);
		targetPos_.y = 0.0f;

		// �J�����̈ʒu�̍X�V
		// �J�����̌����Ă��锽�Ε���
		float revRad = AsoUtility::Deg2RadF(180.0f);			// Y���ɔ��]�������x�N�g��
		dirX = sin(angles_.y + revRad);
		dirZ = cos(angles_.y + revRad);

		// ���K��
		dir = VNorm({ dirX, 0.0f, dirZ });

		// �����ɋ����������Ĉړ��ʂ��擾����
		movePow = VScale(dir, DIS_TARGET_TO_CAMERA);
		movePow = VAdd(movePow, { 0.0f, HEIGHT, 0.0f });		// movePow.y += HEIGHT

		// �����_�Ɉړ��ʂ𑫂��ăJ�����ʒu�Ƃ���
		pos_ = VAdd(targetPos_, movePow);

		// �J�����ʒu
		SetCameraPositionAndTargetAndUpVec(pos_, targetPos_, { 0.0f, 1.0f, 0.0f });
	}
}

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

void Camera::SetUnit(Unit* unit)
{
	unit_ = unit;
}

VECTOR Camera::GetPos(void)
{
	return pos_;
}

VECTOR Camera::GetAngles(void)
{
	return angles_;
}

VECTOR Camera::GetTargetPos(void)
{
	return targetPos_;
}
