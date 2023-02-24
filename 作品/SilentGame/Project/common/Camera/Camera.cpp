#include <DxLib.h>
#include <algorithm>
#include "CameraMode/MoveEventModeCamera.h"
#include "CameraMode/TraceModeCamera.h"
#include "CameraMode/StartEventMode.h"
#include "Camera.h"
#include "../ImageMng.h"
#include "../../Object/Obj.h"
#include "../../Scene/SceneMng.h"
#include "../../Resource/Tmx/TmxObj.h"
#include "../Debug.h"


bool Camera::Init(bool isSmallMap, std::shared_ptr<TmxObj>& tmxObj, const std::weak_ptr<Obj>& traceTarget)
{
	bool rtn{ Init(isSmallMap, tmxObj) };
	StartTraceMode(traceTarget);
	return rtn;
}



bool Camera::Init(Controller& controller, bool isSmallMap, std::shared_ptr<TmxObj>& tmxObj, const std::weak_ptr<Obj>& start, const std::weak_ptr<Obj>& end)
{
	bool rtn{ Init(isSmallMap, tmxObj) };
	StartEvent(controller,start,end);
	return rtn;
}

bool Camera::Update(float delta)
{
	camMode_->Update(*this,delta);
	return true;
}

Math::Vector2I Camera::GetViewSize()
{
	return viewsize_;
}

const Math::Vector2 Camera::GetOffset(void) const
{
	// 初期位置からの座標を引いて描画用オフセットの値を返す
	return defPos_ - pos_;
}

void Camera::StartTraceMode(const std::weak_ptr<Obj>& traceTarget)
{
	camMode_ = std::make_unique<TraceModeCamera>(traceTarget);
	nowMode_ = CamMode::Trace;
	cameraTime_ = 0.0f;
	Update(0.0f);
}

void Camera::StartMoveEvent(const std::weak_ptr<Obj>& start, const std::weak_ptr<Obj>& end, float allTime, float waitTime)
{
	camMode_ = std::make_unique<MoveEventModeCamera>(allTime, waitTime, start, end);
	nowMode_ = CamMode::MoveEvent;
	cameraTime_ = 0.0f;
	Update(0.0f);
}

void Camera::StartEvent(Controller& controller,const std::weak_ptr<Obj>& start, const std::weak_ptr<Obj>& end)
{
	camMode_ = std::make_unique<StartEventMode>(controller,*this,start,end);
	cameraTime_ = 0.0f;
	nowMode_ = CamMode::StartEvent;
	Update(0.0f);
}


void Camera::Draw(void)
{
	camMode_->Draw(*this);
}

bool Camera::Init(bool isSmallMap, std::shared_ptr<TmxObj>& tmxObj)
{
	isSmallMap_ = isSmallMap;
	viewsize_ = { 960,540 };

	// マップのサイズを取得
	auto worldSize = tmxObj->GetWorldArea() * tmxObj->GetTileSize();

	// 上方向の制限
	// 左方向の制限
	rect_.leftTop = { viewsize_ / 2.0f };

	// 下方向の制限
	// 右方向の制限
	rect_.rightBottom = { static_cast<Math::Vector2>(worldSize) - viewsize_ / 2.0f };

	// カメラ初期位置
	defPos_ = { rect_.leftTop };
	pos_ = defPos_;
	return true;
}

void Camera::Loaded(void)
{
	camMode_->Loaded();
}



