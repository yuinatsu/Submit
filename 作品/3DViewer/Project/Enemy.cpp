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

	// モデルの座標を設定
	MV1SetPosition(modelID_, pos_);

	// モデルの角度を設定
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

	// 2つの座標をピタゴラスの定理を使って指定距離いないかチェック
	float distance = std::pow(diff.x, 2.0f) + std::pow(diff.z, 2.0f);
	if (distance < std::pow(VIEW_RANGE, 2.0f))
	{
		// 演習③の角度の取り方を参考にユニットが指定角度ないかチェック

		// 自分から見たプレイヤーの角度を求める
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


	// モデルを描画
	MV1DrawModel(modelID_);

	float viewRad = AsoUtility::Deg2RadF(VIEW_ANGLE);

	// 向き角度から方向を取得する
	float x1 = sin(angles_.y);
	float z1 = cos(angles_.y);

	float x2 = sin(angles_.y - viewRad);
	float z2 = cos(angles_.y - viewRad);

	float x3 = sin(angles_.y + viewRad);
	float z3 = cos(angles_.y + viewRad);


	// 自分の位置
	VECTOR pos0 = pos_;

	// 正面の位置
	VECTOR pos1 = pos_;
	pos1.x += (x1 * VIEW_RANGE);
	pos1.z += (z1 * VIEW_RANGE);

	// 正面の位置
	VECTOR pos2 = pos_;
	pos2.x += (x2 * VIEW_RANGE);
	pos2.z += (z2 * VIEW_RANGE);

	// 正面の位置
	VECTOR pos3 = pos_;
	pos3.x += (x3 * VIEW_RANGE);
	pos3.z += (z3 * VIEW_RANGE);

	// 視野の描画
	DrawLine3D(pos0, pos1, 0x000000);
	DrawLine3D(pos0, pos2, 0x000000);
	DrawLine3D(pos0, pos3, 0x000000);


	// !DrawTriangle3Dの引数の座標は時計回りの指定する
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
