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

	// モデルの座標を設定
	MV1SetPosition(modelID_, pos_);

	// モデルの角度を設定
	VECTOR angles = angles_;
	angles.x += anglesLocal_.x;
	angles.y += anglesLocal_.y;
	angles.z += anglesLocal_.z;
	MV1SetRotationXYZ(modelID_, angles);

	// 再生するアニメーションの設定
	animWalkID_ = MV1AttachAnim(modelID_, 1);

	// アニメーション総再生時間の取得
	timeTotalAnimWalk_ = MV1GetAttachAnimTotalTime(modelID_, animWalkID_);
	stepAnim_ = 0.0f;

	MV1SetAttachAnimTime(modelID_, animWalkID_, stepAnim_);
}

void Unit::Update()
{
	memcpy(keyBufOld, keyBuf, sizeof(keyBufOld));			// keyBufをkeyBufOldにコピー
	GetHitKeyStateAll(keyBuf);								// 全てのキーの状態をkeyBufに格納
	for (const auto& bulet : bulet_)
	{
		bulet->Update();
	}

	// 経過時間を取得
	float deltaTime = sceneMng_->GetDeltaTime();

	// 新しいアニメーション再生時間をセット
	stepAnim_ += (deltaTime * SPEED_ANIM);
	if (stepAnim_ > timeTotalAnimWalk_)
	{
		// アニメーションをループ
		stepAnim_ = 0.0f;
	}
	MV1SetAttachAnimTime(modelID_, animWalkID_, stepAnim_);

	float movePow = 5.0f;
	float moveHPow = movePow / 2;
	float rad = 0.0f;

	// 操作キー判定
	bool isHitMove = false;

	// 回転したい角度
	float rotRad = 0.0f;

	// 縦移動--------------------
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
	// 横移動--------------------
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
	// 弾発射
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

		// キャラクターが向いている方向に移動する
		//pos_.x += sinf(angles_.y + rotRad) * moveHPow;
		//pos_.z += cosf(angles_.y + rotRad) * moveHPow;

		// カメラが向いている方向に移動する
		pos_.x += sinf(cameraAngles.y + rotRad) * moveHPow;
		pos_.z += cosf(cameraAngles.y + rotRad) * moveHPow;

		// 移動方向にキャラクターの角度を即座に変える
		//angles_.y = cameraAngles.y + rotRad;

		// 移動方向にキャラクターの角度を徐々に変える
		float radUnitAnglesY = angles_.y;
		float radMoveAnglesY = cameraAngles.y + rotRad;
		radMoveAnglesY = AsoUtility::RadIn2PI(radMoveAnglesY);

		// 回転量が少ない方の回転方向を取得する（時計回り：１、反時計回り：ー１）
		float aroundDir = AsoUtility::DirNearAroundRad(radUnitAnglesY, radMoveAnglesY);

		// ブルブル問題解決→しきい値を設ける
		float diff = radMoveAnglesY - radUnitAnglesY;
		float absDiff = abs(diff);
		if (absDiff < 0.1f)
		{
			// しきい値以内
			angles_.y = radMoveAnglesY;
		}
		else
		{
			// 回転量を加える
			angles_.y += (SPEED_ROT_RAD * aroundDir);
		}

		// クルクル問題解決→angles_.yも0~360度以内に収める
		angles_.y = AsoUtility::RadIn2PI(angles_.y);
	}

	// モデルの角度を設定
	MV1SetPosition(modelID_, pos_);
	VECTOR angles = angles_;
	angles.x += anglesLocal_.x;
	angles.y += anglesLocal_.y;
	angles.z += anglesLocal_.z;
	MV1SetRotationXYZ(modelID_, angles);

	// モデルの座標を設定
	MV1SetPosition(modelID_, pos_);
}

void Unit::Draw()
{
	for (const auto& bulet : bulet_)
	{
		bulet->Draw();
	}
	// モデルを描画
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
