#include <DxLib.h>
#include "../../../Scene/SceneMng.h"
#include "../../../Object/Obj.h"
#include "MoveEventModeCamera.h"
#include "../Camera.h"
#include "../../../Input/Controller.h"
#include "../../ImageMng.h"
#include "../../../Input/InputConfig.h"
#include "StartEventMode.h"

#include "../../Debug.h"

// ポイント間の移動時間
constexpr float moveTime{ 2.5f };

// ターゲットへの移動時間
constexpr float moveTargetTime{ 1.5f };

// 待機時間
constexpr float waitTime{ 0.5f };

constexpr float allTime{ (moveTime * 3.0f) + waitTime + (moveTargetTime * 2.0f) };

StartEventMode::StartEventMode(Controller& controller,Camera& camera, const std::weak_ptr<Obj>& start, const std::weak_ptr<Obj>& end) :
	start_{start}, end_{end}, skipVal_{1.0f}, controller_{controller}
{
	lpImageMng.GetID("Resource/image/Skip.png", "Skip");
	frameVal_ = 1.0f;
	positions_.reserve(4);
	positions_.emplace_back(camera.rect_.leftTop);
	positions_.emplace_back(Math::Vector2{ camera.rect_.rightBottom.x, camera.rect_.leftTop.y});
	positions_.emplace_back(camera.rect_.rightBottom);
	positions_.emplace_back(Math::Vector2{ camera.rect_.leftTop.x, camera.rect_.rightBottom.y });

	if (camera.isSmallMap_)
	{
		posItr_ = --positions_.end();
		updateFunc_ = std::bind(&StartEventMode::MoveTargetUpdate, this, std::placeholders::_1, std::placeholders::_2);
		endPos_ = *posItr_;
	}
	else
	{
		posItr_ = positions_.begin();
		updateFunc_ = std::bind(&StartEventMode::MoveMapUpdate, this, std::placeholders::_1, std::placeholders::_2);
		endPos_ = *(posItr_ + 1);
	}

	startPos_ = *posItr_;
	
	skipTime_ = 0.0f;
}

StartEventMode::~StartEventMode()
{
}

void StartEventMode::Update(Camera& camera, float delta)
{
	skipTime_ += delta;
	if (skipTime_ >= 1.3f)
	{
		skipTime_ = 0.0f;
	}
	controller_.Update(delta);
	auto& t = lpImageMng.GetInstance();
	updateFunc_(camera,delta);
}

void StartEventMode::Draw(Camera& camera)
{
	constexpr int frameWidth{ 100 };
	const int frame = static_cast<int>(static_cast<float>(frameWidth) * frameVal_);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, lpSceneMng.GetScreenSize().x, frame, 0x000000, true);
	DrawBox(0, lpSceneMng.GetScreenSize().y - frame, lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	if (skipTime_ < 0.8)
	{

		DrawGraph(static_cast<int>(skipPos_.x), static_cast<int>(skipPos_.y), lpImageMng.GetID("Skip")[0], true);
		DrawGraph(static_cast<int>(skipPos_.x) + inputOffset_, static_cast<int>(skipPos_.y), lpImageMng.GetID(skipInput_)[inputCode_], true);
	}
}

void StartEventMode::MoveMapUpdate(Camera& camera, float delta)
{
	if (controller_.Released(InputID::Attack) && camera.cameraTime_ >= 0.5f)
	{
		Skip(camera);
		return;
	}
	
	camera.cameraTime_ += delta;
	float now{ camera.cameraTime_ / moveTime };
	camera.pos_ = Math::Learp(startPos_, endPos_, now);
	if (now >= 1.0f)
	{
		camera.cameraTime_ = 0.0f;
		++posItr_;
		if ((posItr_ + 1) == positions_.end())
		{
			updateFunc_ = std::bind(&StartEventMode::MoveTargetUpdate, this, std::placeholders::_1,std::placeholders::_2);
			startPos_ = *posItr_;
			return;
		}
		startPos_ = *posItr_;
		endPos_ = *(posItr_ + 1);
	}
}

void StartEventMode::MoveTargetUpdate(Camera& camera, float delta)
{
	if (controller_.Released(InputID::Attack) && camera.cameraTime_ >= 0.5f)
	{
		Skip(camera);
		return;
	}

	camera.cameraTime_ += delta;
	float now{ camera.cameraTime_ / moveTargetTime };
	if (!end_.expired())
	{
		endPos_ = end_.lock()->GetPos();
		camera.pos_ = Math::Learp(startPos_, endPos_, now);
	}

	if (now >= 1.0f)
	{
		camera.cameraTime_ = 0.0f;
		updateFunc_ = std::bind(&StartEventMode::WaitTargetUpdate, this, std::placeholders::_1,std::placeholders::_2);
		startPos_ = camera.pos_;
	}
}

void StartEventMode::WaitTargetUpdate(Camera& camera, float delta)
{
	if (controller_.Released(InputID::Attack))
	{
		Skip(camera);
		return;
	}
	camera.cameraTime_ += delta;
	if (waitTime <= camera.cameraTime_)
	{
		camera.cameraTime_ = 0.0f;
		updateFunc_ = std::bind(&StartEventMode::BackUpdate, this, std::placeholders::_1,std::placeholders::_2);
	}
}

void StartEventMode::BackUpdate(Camera& camera, float delta)
{
	skipTime_ = 1.3f;
	// 時間を更新
	camera.cameraTime_ += delta * skipVal_;

	// 時間を最大時間で割って0から1に
	float now{ camera.cameraTime_ / moveTargetTime };

	// 上下にあるフレーム分の計算
	frameVal_ = Math::Clamp((moveTargetTime -camera.cameraTime_) / moveTargetTime / (1.0f / 5.0f), 0.0f, 1.0f);
	
	if (!start_.expired())
	{
		// 終了地点を計算
		endPos_ = { Math::Clamp(start_.lock()->GetPos(),camera.rect_.leftTop,camera.rect_.rightBottom) };
		
		// 計算した値で線形補間
		camera.pos_ = Math::Learp(startPos_, endPos_, now);
	}
	if (now >= 1.0f)
	{
		// 計算した値が1.0f以上の時
		camera.cameraTime_ = 0.0f;
		lpImageMng.Relese("Skip");
		camera.StartTraceMode(start_);
	}
}

void StartEventMode::Skip(Camera& camera)
{
	DebugLog("スキップ");
	skipVal_ = 2.0f;
	camera.cameraTime_ = 0.0f;
	startPos_ = camera.pos_;
	updateFunc_ = std::bind(&StartEventMode::BackUpdate, this, std::placeholders::_1, std::placeholders::_2);
}

void StartEventMode::Loaded(void)
{
	int x, y;
	GetGraphSize(lpImageMng.GetID("Skip")[0], &x, &y);
	
	int x1, y1;
	if (controller_.GetCntType() == CntType::Key)
	{
		skipInput_ = "Keydata";
		inputCode_ = lpConfigMng.GetKeyInputCode().at(InputID::Attack);
	}
	else
	{
		auto type = GetJoypadType(DX_INPUT_PAD1);
		if (type == 3 || type == 4)
		{
			skipInput_ = "PadPS";
		}
		else
		{
			skipInput_ = "PadXbox";
		}
		inputCode_ = lpConfigMng.GetJoypadInputCode().at(InputID::Attack);
	}
	GetGraphSize(lpImageMng.GetID(skipInput_)[0], &x1, &y1);

	skipPos_.x = (lpSceneMng.GetScreenSize().x  - static_cast<float>(x + x1))/2.0f;
	skipPos_.y = lpSceneMng.GetScreenSize().y - y1 * 2.0f;
	inputOffset_ = x;
}
