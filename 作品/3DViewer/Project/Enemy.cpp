#include <cmath>
#include "Enemy.h"
#include "Unit.h"
#include "AsoUtility.h"

Enemy::Enemy(SceneManager* manager, Unit* unit)
{
	sceneMng_ = manager;
	unit_ = unit;
}

void Enemy::Init()
{
	modelID_ = MV1LoadModel("Model/Human.mv1");
	pos_ = { 200.0f, 0.0f, 400.0f };
	angles_ = { 0.0f, AsoUtility::Deg2RadF(42.0f), 0.0f };
	anglesLocal_ = { 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f };

	// ���f���̍��W��ݒ�
	MV1SetPosition(modelID_, pos_);

	// ���f���̊p�x��ݒ�
	VECTOR angles = angles_;
	angles.x += anglesLocal_.x;
	angles.y += anglesLocal_.y;
	angles.z += anglesLocal_.z;
	MV1SetRotationXYZ(modelID_, angles);

	isNotice_ = false;
}

void Enemy::Update()
{
	isNotice_ = false;

	VECTOR uPos = unit_->GetPos();
	VECTOR diff = VSub(uPos, pos_);

	// 2�̍��W���s�^�S���X�̒藝���g���Ďw�苗�����Ȃ����`�F�b�N
	float distance = std::pow(diff.x, 2.0f) + std::pow(diff.z, 2.0f);
	if (distance < std::pow(VIEW_RANGE, 2.0f))
	{
		// ���K�B�̊p�x�̎������Q�l�Ƀ��j�b�g���w��p�x�Ȃ����`�F�b�N

		// �������猩���v���C���[�̊p�x�����߂�
		float rad = atan2(diff.x, diff.z);
		float viewRad = rad - angles_.y;
		float viewDeg = AsoUtility::DegIn360(AsoUtility::Rad2DegF(viewRad));
		if (viewDeg <= VIEW_ANGLE || viewDeg >= (360.0f - VIEW_ANGLE))
		{
			isNotice_ = true;
		}
	}
}

void Enemy::Draw()
{
	if (isNotice_)
	{
		MV1SetMaterialDifColor(modelID_, 0, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		MV1SetMaterialDifColor(modelID_, 0, GetColorF(0.5f, 0.5f, 0.5f, 1.0f));
	}


	// ���f����`��
	MV1DrawModel(modelID_);

	float viewRad = AsoUtility::Deg2RadF(VIEW_ANGLE);

	// �����p�x����������擾����
	float x1 = sin(angles_.y);
	float z1 = cos(angles_.y);

	float x2 = sin(angles_.y - viewRad);
	float z2 = cos(angles_.y - viewRad);

	float x3 = sin(angles_.y + viewRad);
	float z3 = cos(angles_.y + viewRad);


	// �����̈ʒu
	VECTOR pos0 = pos_;

	// ���ʂ̈ʒu
	VECTOR pos1 = pos_;
	pos1.x += (x1 * VIEW_RANGE);
	pos1.z += (z1 * VIEW_RANGE);

	// ���ʂ̈ʒu
	VECTOR pos2 = pos_;
	pos2.x += (x2 * VIEW_RANGE);
	pos2.z += (z2 * VIEW_RANGE);

	// ���ʂ̈ʒu
	VECTOR pos3 = pos_;
	pos3.x += (x3 * VIEW_RANGE);
	pos3.z += (z3 * VIEW_RANGE);

	// ����̕`��
	DrawLine3D(pos0, pos1, 0x000000);
	DrawLine3D(pos0, pos2, 0x000000);
	DrawLine3D(pos0, pos3, 0x000000);


	// !DrawTriangle3D�̈����̍��W�͎��v���̎w�肷��
	pos0.y = pos1.y = pos2.y = pos3.y = 1;
	DrawTriangle3D(pos0, pos2, pos1, 0xffdead, true);
	DrawTriangle3D(pos0, pos1, pos3, 0xffdead, true);
}

void Enemy::Release()
{
	MV1DeleteModel(modelID_);
}

VECTOR Enemy::GetPos(void)
{
    return pos_;
}

VECTOR Enemy::GetAngles(void)
{
    return angles_;
}
