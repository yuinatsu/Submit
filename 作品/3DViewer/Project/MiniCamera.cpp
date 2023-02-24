#include "DxLib.h"
#include "Camera.h"
#include "GameCommon.h"
#include "MiniCamera.h"

MiniCamera::MiniCamera(Camera* camera)
{
	camera_ = camera;
	size_ = { 200, 200 };
	screen_ = MakeScreen(size_.x, size_.y);
}

MiniCamera::~MiniCamera()
{
}

void MiniCamera::Init(void)
{
	model_ = MV1LoadModel("Model/Camera.mv1");
}

void MiniCamera::Update(void)
{

	VECTOR pos = camera_->GetPos();
	pos.y += 100.0f;

	MV1SetPosition(model_, camera_->GetPos());
	MV1SetScale(model_, { 3.0f, 3.0f ,3.0f });

	VECTOR angles = camera_->GetAngles();
	MV1SetRotationXYZ(model_, angles);

}

void MiniCamera::DrawScreen(void)
{

	SetDrawScreen(screen_);

	// 画面を初期化
	ClearDrawScreen();

	VECTOR pos = camera_->GetPos();
	pos.z -= 500.0f;

	// カメラセット
	SetCameraPositionAndAngle(
		pos,
		0.0f,
		0.0f,
		0.0f
	);

	// ライトの設定
	ChangeLightTypeDir({ 0.0f, -1.0f, 0.0f });

	MV1DrawModel(model_);

	// ライトの設定
	ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

}

void MiniCamera::Draw(void)
{

	int width = 10;
	DrawBox(
		SCREEN_SIZE_X - size_.x - (width * 2),
		0,
		SCREEN_SIZE_X,
		size_.y + (width * 2),
		0x000000,
		true
	);
	DrawGraph(SCREEN_SIZE_X - size_.x - width, width, screen_, false);

}

void MiniCamera::Release(void)
{
	MV1DeleteModel(model_);
}

int MiniCamera::GetScreen(void)
{
	return screen_;
}
