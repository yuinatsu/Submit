#include "Camera.h"
#include "Math.h"
#include "Input/Controller.h"
#include <EffekseerForDXLib.h>

Camera::Camera()
{
	rotation_ = MGetIdent();
	// カメラの注視点
	target_ = Vector3{ 0,0,0 };
	speed_ = { 0.0f,0.0f };
	yaw = 0;
	SetBefore();
}

// カメラのセットアップ
void Camera::SetUpScreen(void) const
{
	auto vec = VTransform(VGet(0.0f, 0.0f, 1.0f), rotation_);
	auto up = VTransform(VGet(0.0f, 1.0f, 0.0f), rotation_);
	SetCameraPositionAndTargetAndUpVec(VGet(pos_.x, pos_.y, pos_.z), vtarget_, up);
}

// カメラを指定座標の方に向ける
void Camera::Look(const Vector3& pos)
{
	auto vec = VTransform(VGet(forwardVector3<float>.x, forwardVector3<float>.y, forwardVector3<float>.z), rotation_);
	auto lookVec = pos_ - pos;
	rotation_ = MGetRotVec2(vec, VGet(lookVec.x, lookVec.y, lookVec.z));
}

const Vector3 Camera::GetUp(void) const
{
	auto vec = VTransform(VGet(upVector3<float>.x, upVector3<float>.y, upVector3<float>.z), rotation_);
	return {vec.x, vec.y, vec.z};
}

const Vector3 Camera::GetForward(void) const
{
	auto vec = VTransform(VGet(forwardVector3<float>.x, forwardVector3<float>.y, forwardVector3<float>.z), rotation_);
	return { vec.x, vec.y, vec.z };
}

void Camera::rotate(float* posX, float* posY, const float angle, const Vector3 mpos)
{
	const float ox = *posX - mpos.x, oy = *posY - mpos.y;
	*posX = ox * cos(angle) + oy * sin(angle);
	*posX += mpos.x;
}

void Camera::ControlCamera(Controller& controller)
{
	mousePos_ = controller.GetCursorPos();
	if (HALF_SIZE_X < mousePos_.x)
	{
		speed_.x = -Deg2Rad(0.01f);
	}
	else if (HALF_SIZE_X > mousePos_.x)
	{
		speed_.x = Deg2Rad(0.01f);
	}
	else
	{
		speed_.x = 0.0f;
	}
	controller.SetCursorPos();
	rotate(&angle_.x, &angle_.y, speed_.x, pos_);
}

void Camera::SetBefore(void)
{
	// 注視点の移動
	Vector2 div = Vector2(sin(angle_.x), cos(angle_.x));
	// 正規化
	auto dir = VNorm({ div.x,0.0f,div.y });
	// 方向に距離をかけて、移動量を取得
	//auto movePos = VScale(dir, TARGET_TO_CHAR);
	//vtarget_ = VAdd(VGet(pPos_.x, pPos_.y, pPos_.z), movePos);
	vtarget_ = VGet(pPos_.x, pPos_.y, pPos_.z);
	//vtarget_.y = + pPos_.y;
	// カメラの向いている反対方向
	float revRad = Deg2Rad(180.0f);
	div.x = sin(angle_.x + revRad);
	div.y = cos(angle_.x + revRad);
	// 正規化
	dir = VNorm({ div.x,0.0f,div.y });

	// 方向に距離をかけて、移動量を取得
	auto movePos = VScale(dir, TARGET_TO_CAMERA);
	movePos.y += HEGHT;
	auto newPos = VAdd(vtarget_, movePos);
	pos_ = Vector3{ newPos.x,newPos.y,newPos.z};

	Effekseer_Sync3DSetting();
}

void Camera::Update(Controller& controller)
{
	ControlCamera(controller);
	SetBefore();
}

Vector3 Camera::GetAngle(void)
{
	return Vector3(angle_.x,angle_.y,angle_.z);
}
