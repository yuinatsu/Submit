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
	// カメラの初期設定
	pos_ = { 0.0f, HEIGHT, -DIS_TARGET_TO_CAMERA };

	// カメラ角度(rad)
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
	// キーボード操作でカメラ位置を移動
	float rad = 0.0f;
	//// 縦移動--------------------
	//if (CheckHitKey(KEY_INPUT_W))
	//{
	//	// ワールド座標の前方
	//	// Z正方向
	//	//pos_.z += movePow;
	//	// カメラの前方
	//	rad = AsoUtility::Deg2RadD(0.0f);
	//	pos_.x += sinf(angles_.y + rad) * moveHPow;
	//	pos_.z += cosf(angles_.y + rad) * moveHPow;
	//}
	//if (CheckHitKey(KEY_INPUT_S))
	//{
	//	// ワールド座標の後方
	//	// Z負方向
	//	//pos_.z -= movePow;
	//	// カメラの後方
	//	rad = AsoUtility::Deg2RadD(0.0f);
	//	pos_.x -= sinf(angles_.y + rad) * moveHPow;
	//	pos_.z -= cosf(angles_.y + rad) * moveHPow;
	//}
	//// 横移動--------------------
	//if (CheckHitKey(KEY_INPUT_D))
	//{
	//	// ワールド座標の右側
	//	// X正方向
	//	//pos_.x += movePow;
	//	// カメラの右方向
	//	rad = AsoUtility::Deg2RadD(-90.0);
	//	pos_.x -= sinf(angles_.y + rad) * moveHPow;
	//	pos_.z -= cosf(angles_.y + rad) * moveHPow;
	//}
	//if (CheckHitKey(KEY_INPUT_A))
	//{
	//	// ワールド座標の左側
	//	// X負方向
	//	//pos_.x -= movePow;
	//	// カメラの左方向
	//	rad = AsoUtility::Deg2RadD(-90.0);
	//	pos_.x += sinf(angles_.y + rad) * moveHPow;
	//	pos_.z += cosf(angles_.y + rad) * moveHPow;
	//}

	//// 垂直移動------------------
	//if (CheckHitKey(KEY_INPUT_N))
	//{
	//	// Y正方向
	//	pos_.y += movePow;
	//}
	//if (CheckHitKey(KEY_INPUT_M))
	//{
	//	// Y負方向
	//	pos_.y -= movePow;
	//}

	// キーボード操作でカメラ角度を変更
	// 横回転----------------------
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		// 右回転
		angles_.y += AsoUtility::Deg2RadD(1.0f);
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		// 左回転
		angles_.y -= AsoUtility::Deg2RadD(1.0f);
	}
	//// 縦回転-----------------------
	//if (CheckHitKey(KEY_INPUT_DOWN))
	//{
	//	// 上回転
	//	angles_.x += AsoUtility::Deg2RadD(1.0f);
	//}
	//if (CheckHitKey(KEY_INPUT_UP))
	//{
	//	// 下回転
	//	angles_.x -= AsoUtility::Deg2RadD(1.0f);
	//}
}

void Camera::SetBeforDraw(void)
{
	if (unit_ == nullptr)
	{
		// カメラ情報セット
		SetCameraPositionAndAngle(pos_, angles_.x, angles_.y, angles_.z);
	}
	else
	{
		float dirX;
		float dirZ;
		VECTOR dir;
		VECTOR movePow;

		// カメラの向いている方向
		dirX = sin(angles_.y);
		dirZ = cos(angles_.y);

		// 正規化
		dir = VNorm({ dirX, 0.0f, dirZ });

		// 方向から移動量を算出
		movePow = VScale(dir, DIS_TARGET_TO_UNIT);

		VECTOR uPos = unit_->GetPos();
		targetPos_ = VAdd(uPos, movePow);
		targetPos_.y = 0.0f;

		// カメラの位置の更新
		// カメラの向いている反対方向
		float revRad = AsoUtility::Deg2RadF(180.0f);			// Y軸に反転させたベクトル
		dirX = sin(angles_.y + revRad);
		dirZ = cos(angles_.y + revRad);

		// 正規化
		dir = VNorm({ dirX, 0.0f, dirZ });

		// 方向に距離をかけて移動量を取得する
		movePow = VScale(dir, DIS_TARGET_TO_CAMERA);
		movePow = VAdd(movePow, { 0.0f, HEIGHT, 0.0f });		// movePow.y += HEIGHT

		// 注視点に移動量を足してカメラ位置とする
		pos_ = VAdd(targetPos_, movePow);

		// カメラ位置
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
