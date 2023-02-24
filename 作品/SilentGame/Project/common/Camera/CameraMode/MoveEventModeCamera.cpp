#include <algorithm>
#include <DxLib.h>
#include "TraceModeCamera.h"
#include "../../../Scene/SceneMng.h"
#include "../Camera.h"
#include "../../../Object/Obj.h"
#include "MoveEventModeCamera.h"


MoveEventModeCamera::MoveEventModeCamera(float allTime, float waitTime,const std::weak_ptr<Obj>& start,const std::weak_ptr<Obj>& end) :
	allTime_{allTime}, waitTime_{waitTime}, start_{start}, end_{end}
{
}

MoveEventModeCamera::~MoveEventModeCamera()
{
}

void MoveEventModeCamera::Update(Camera& camera, float delta)
{
	camera.cameraTime_ += delta;
	// 移動中のカメラの状態
	if (eventCamState_ == EventCamState::Move)
	{
		// ターゲットに移動中の時

		// 時間を計算
		float now{ camera.cameraTime_ / ((allTime_ - waitTime_) / 2.0f) };

		// 追跡するオブジェクトとターゲットが生きているか？
		if (!start_.expired() && !end_.expired())
		{
			// 両方生きているとき時間値を使いターゲットの方へ移動する
			Math::Vector2 clampPos{ Math::Clamp(start_.lock()->GetPos(),camera.rect_.leftTop,camera.rect_.rightBottom)};
			camera.pos_ = clampPos + ((end_.lock()->GetPos() - clampPos) * now);
		}

		// 値をチェック
		if (now >= 1.0f)
		{
			// 正規化した時間値が1以上の時はターゲットにとうちゃくしたのbackに
			eventCamState_ = EventCamState::Wait;
			camera.cameraTime_ = 0.0f;
		}
	}
	else if (eventCamState_ == EventCamState::Wait)
	{
		// 時間を計算
		float now{ camera.cameraTime_ / waitTime_ };

		// 値をチェック
		if (now >= 1.0f)
		{
			// 正規化した時間値が1以上の時はターゲットにとうちゃくしたのbackに
			eventCamState_ = EventCamState::Back;
			camera.cameraTime_ = 0.0f;
		}
	}
	else
	{
		// 追跡するオブジェクトに戻る時

		// 時間を計算
		float now = (camera.cameraTime_ / ((allTime_ - waitTime_) / 2.0f));

		// 追跡するオブジェクトとターゲットが生きているか？
		if (!start_.expired() && !end_.expired())
		{
			// 生きているときターゲットから追跡するオブジェクトへ時間値を使い移動する
			Math::Vector2 clampPos{ Math::Clamp(start_.lock()->GetPos(),camera.rect_.leftTop,camera.rect_.rightBottom) };
			camera.pos_ = end_.lock()->GetPos() + ((clampPos - end_.lock()->GetPos()) * now);
		}

		// 値チェック
		if (now >= 1.0f)
		{
			// 1以上の時カメラの修正位置への移動を開始する
			eventCamState_ = EventCamState::Move;
			camera.nowMode_ = CamMode::Trace;
			camera.cameraTime_ = 0.0f;
			camera.camMode_ = std::make_unique<TraceModeCamera>(start_);
		}
	}
}

void MoveEventModeCamera::Draw(Camera& camera)
{
	// 上下に黒色で透明な枠を出す
	constexpr int frameWidth{ 100 };
	const float timeMax{ ((allTime_ - waitTime_) / 2.0f) };
	float now = (camera.cameraTime_ / timeMax) / (1.0f / 5.0f);
	if (eventCamState_ == EventCamState::Back)
	{
		now = ((timeMax - camera.cameraTime_) / timeMax) / (1.0f / 5.0f);
	}
	else if (eventCamState_ == EventCamState::Wait)
	{
		now = 1.0f;
	}
	now = Math::Clamp(now, 0.0f, 1.0f);
	int frame = static_cast<int>(static_cast<float>(frameWidth) * now);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, lpSceneMng.GetScreenSize().x, frame, 0x000000, true);
	DrawBox(0, lpSceneMng.GetScreenSize().y - frame, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
