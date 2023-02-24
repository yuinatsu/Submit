#pragma once
#include <memory>
#include "CameraModeBase.h"

class Obj;

// 往復移動するカメラ
class MoveEventModeCamera :
	public CameraModeBase
{
public:
	MoveEventModeCamera(float allTime,float waitTime,const std::weak_ptr<Obj>& start,const std::weak_ptr<Obj>& end);
	~MoveEventModeCamera();
	void Update(Camera& camera, float delta) final;
	void Draw(Camera& camera) final;
private:

	// 開始位置
	std::weak_ptr<Obj> start_;

	// 片道の終点
	std::weak_ptr<Obj> end_;

	// カメラのモードの時間
	float allTime_;

	// 待機時間
	float waitTime_;

	// ターゲットの方へ往復するカメラの状態
	enum class EventCamState
	{
		Move,		// 移動中
		Wait,
		Back		// 戻る
	};

	// ターゲットの方へ往復するカメラの現在の状態
	EventCamState eventCamState_{ EventCamState::Move };
};

