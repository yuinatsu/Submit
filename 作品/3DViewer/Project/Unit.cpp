#include "Unit.h"
#include "SceneManager.h"
#include "AsoUtility.h"
#include "Camera.h"
#include "Bullet.h"
#include "Application.h"

Unit::Unit(SceneManager* manager)
{
	sceneMng_ = manager;
}

void Unit::Init()
{
	modelID_ = MV1LoadModel("Model/Walking.mv1");
	pos_ = { 0.0f, 0.0f, 0.0f };
	angles_ = { 0.0f, 0.0f, 0.0f };
	anglesLocal_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// ���f���̍��W��ݒ�
	MV1SetPosition(modelID_, pos_);

	// ���f���̊p�x��ݒ�
	VECTOR angles = angles_;
	angles.x += anglesLocal_.x;
	angles.y += anglesLocal_.y;
	angles.z += anglesLocal_.z;
	MV1SetRotationXYZ(modelID_, angles);

	// �Đ�����A�j���[�V�����̐ݒ�
	animWalkID_ = MV1AttachAnim(modelID_, 1);

	// �A�j���[�V�������Đ����Ԃ̎擾
	timeTotalAnimWalk_ = MV1GetAttachAnimTotalTime(modelID_, animWalkID_);
	stepAnim_ = 0.0f;

	MV1SetAttachAnimTime(modelID_, animWalkID_, stepAnim_);
}

void Unit::Update()
{
	memcpy(keyBufOld, keyBuf, sizeof(keyBufOld));			// keyBuf��keyBufOld�ɃR�s�[
	GetHitKeyStateAll(keyBuf);								// �S�ẴL�[�̏�Ԃ�keyBuf�Ɋi�[
	for (const auto& bulet : bulet_)
	{
		bulet->Update();
	}

	// �o�ߎ��Ԃ��擾
	float deltaTime = sceneMng_->GetDeltaTime();

	// �V�����A�j���[�V�����Đ����Ԃ��Z�b�g
	stepAnim_ += (deltaTime * SPEED_ANIM);
	if (stepAnim_ > timeTotalAnimWalk_)
	{
		// �A�j���[�V���������[�v
		stepAnim_ = 0.0f;
	}
	MV1SetAttachAnimTime(modelID_, animWalkID_, stepAnim_);

	float movePow = 5.0f;
	float moveHPow = movePow / 2;
	float rad = 0.0f;

	// ����L�[����
	bool isHitMove = false;

	// ��]�������p�x
	float rotRad = 0.0f;

	// �c�ړ�--------------------
	if (CheckHitKey(KEY_INPUT_W))
	{
		isHitMove = true;
		rotRad = AsoUtility::Deg2RadF(0.0f);
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		isHitMove = true;
		rotRad = AsoUtility::Deg2RadF(180.0f);
	}
	// ���ړ�--------------------
	if (CheckHitKey(KEY_INPUT_D))
	{
		isHitMove = true;
		rotRad = AsoUtility::Deg2RadF(90.0f);
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		isHitMove = true;
		rotRad = AsoUtility::Deg2RadF(-90.0f);
	}
	// �e����
	if (keyBuf[KEY_INPUT_Q] && !keyBufOld[KEY_INPUT_Q])
	{
		auto bulet = std::make_shared<Bullet>(sceneMng_);
		auto pos = pos_;
		pos.y += 120.0f;
		bulet->Init(pos, angles_);
		bulet_.emplace_back(bulet);
	}

	if (isHitMove)
	{
		Camera* camera = sceneMng_->GetCamera();
		VECTOR cameraAngles = camera->GetAngles();

		// �L�����N�^�[�������Ă�������Ɉړ�����
		//pos_.x += sinf(angles_.y + rotRad) * moveHPow;
		//pos_.z += cosf(angles_.y + rotRad) * moveHPow;

		// �J�����������Ă�������Ɉړ�����
		pos_.x += sinf(cameraAngles.y + rotRad) * moveHPow;
		pos_.z += cosf(cameraAngles.y + rotRad) * moveHPow;

		// �ړ������ɃL�����N�^�[�̊p�x�𑦍��ɕς���
		//angles_.y = cameraAngles.y + rotRad;

		// �ړ������ɃL�����N�^�[�̊p�x�����X�ɕς���
		float radUnitAnglesY = angles_.y;
		float radMoveAnglesY = cameraAngles.y + rotRad;
		radMoveAnglesY = AsoUtility::RadIn2PI(radMoveAnglesY);

		// ��]�ʂ����Ȃ����̉�]�������擾����i���v���F�P�A�����v���F�[�P�j
		float aroundDir = AsoUtility::DirNearAroundRad(radUnitAnglesY, radMoveAnglesY);

		// �u���u�����������������l��݂���
		float diff = radMoveAnglesY - radUnitAnglesY;
		float absDiff = abs(diff);
		if (absDiff < 0.1f)
		{
			// �������l�ȓ�
			angles_.y = radMoveAnglesY;
		}
		else
		{
			// ��]�ʂ�������
			angles_.y += (SPEED_ROT_RAD * aroundDir);
		}

		// �N���N����������angles_.y��0~360�x�ȓ��Ɏ��߂�
		angles_.y = AsoUtility::RadIn2PI(angles_.y);
	}

	// ���f���̊p�x��ݒ�
	MV1SetPosition(modelID_, pos_);
	VECTOR angles = angles_;
	angles.x += anglesLocal_.x;
	angles.y += anglesLocal_.y;
	angles.z += anglesLocal_.z;
	MV1SetRotationXYZ(modelID_, angles);

	// ���f���̍��W��ݒ�
	MV1SetPosition(modelID_, pos_);
}

void Unit::Draw()
{
	for (const auto& bulet : bulet_)
	{
		bulet->Draw();
	}
	// ���f����`��
	MV1DrawModel(modelID_);
}

void Unit::Release()
{
	MV1DeleteModel(modelID_);
}

VECTOR Unit::GetPos(void)
{
	return pos_;
}

VECTOR Unit::GetAngles(void)
{
	return angles_;
}
