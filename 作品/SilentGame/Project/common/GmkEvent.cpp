#include <DxLib.h>
#include "GmkEvent.h"
#include "SoundPross.h"
#include "../Object/Unit/Enemy.h"
#include "../Object/ObjManager.h"
#include "../common/ResultData.h"
#include "ImageMng.h"

#include "Debug.h"

// 警戒態勢の最大時間
constexpr float MaxAlert = 20;

// ギミックイベントの持続時間
constexpr float gmkMaxTime{ 15.0f };

// 数字の画像が気に入らなければここを変えると変更できる(ただしサイズは{25,40}にすること)
constexpr char num[]{ "./Resource/Image/number.png" };

GmkEvent::GmkEvent(std::shared_ptr< ResultData>& resultData) :
	resultData_{resultData}
{
	gmkStepTime_ = 0.0f;
	flag_ = false;
	scFlag_ = true;
	sColor_ = 0.0f;
	nowType_ = EventType::Non;
	gmkFunc_.reserve(static_cast<size_t>(EventType::Non) + 1ULL);

	gmkFunc_.emplace(
		EventType::Alert,
			std::bind(&GmkEvent::UpdateAlert, this, std::placeholders::_1));
	gmkFunc_.emplace(
		EventType::Clear,
			std::bind(&GmkEvent::UpdateAlert, this, std::placeholders::_1));
	gmkFunc_.emplace(
		EventType::Non,
		[](float delta) {});
}

GmkEvent::~GmkEvent()
{
	// oundProcess::SoundStop(SoundProcess::SOUNDNAME_SE::waring);
}



void GmkEvent::StartEvent(const Math::Vector2& pos, EventType type)
{
	// 現在の発生中のギミックとのチェック
	if (nowType_ == type || (nowType_ == EventType::Alert && type != EventType::Alert))
	{
		if (nowType_ == EventType::Alert && type == EventType::Alert)
		{
			gmkStepTime_ = gmkMaxTime;
		}
		// 同じギミックもしくは監視カメラの時それ以外のギミックイベントの時処理をやめる
		return;
	}
	nowType_ = type;
	gmkStepTime_ = gmkMaxTime;

	// 見つかった数をカウントアップする
	resultData_->CountUpAlertCount();

	// naviで移動できる敵を探す
	for (auto& e : objMng_.lock()->GetSortedEnemyList(pos))
	{
		// naviによって移動ができるかチェック
		if (dynamic_cast<Enemy&>(*e).StartMoveNavi(pos))
		{
			// できるやつがいればループを抜ける
			DebugLog("移動できる敵を発見");
			break;
		}
	}

	// 敵全体の移動スピードを上げる
	for (auto& e : objMng_.lock()->GetSortedEnemyList(pos))
	{
		e->SetSpMag(1.25f);
	}

	if (EventType::Alert == nowType_)
	{
		for (auto& f : startCollFunc_)
		{
			f();
		}
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_BGM::alarm, SoundProcess::GetBGMVolumeEntire(), true);
	}
}

void GmkEvent::StartAlert()
{
	nowType_ = EventType::Clear;
	gmkStepTime_ = gmkMaxTime;
}

void GmkEvent::EndAlert()
{
	for (auto& e : objMng_.lock()->GetSortedEnemyList(Math::zeroVector2<float>))
	{
		e->SetSpMag(1.0f);
	}
	nowType_ = EventType::Non;
	sColor_ = 0.0f;
	
}

void GmkEvent::Update(float delta)
{
	gmkStepTime_ -= delta;
	gmkFunc_[nowType_](delta);
}

void GmkEvent::Draw(void)
{
	
}

void GmkEvent::MainDraw(void)
{
	if (nowType_ == EventType::Alert)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
		DrawGraph(50, 0, lpImageMng.GetID("./Resource/Image/Alert.png")[0], true);
		auto t = (gmkStepTime_ / gmkMaxTime) * 100;
		if (t >= 99)
		{
			t = 99.99f;
		}

		// まれにtが10になるので応急処置でクランプしてます
		int t1 = std::clamp(static_cast<int>(t / 10),0,9);
		int t2 = std::clamp(static_cast<int>(t - (t1 * 10)),0,9);
		int t3 = std::clamp(static_cast<int>(((t * 10) - ((t1 * 100) + (t2 * 10)))),0,9);
		int t4 = std::clamp(static_cast<int>((t * 100) - ((t1 * 1000) + (t2 * 100) + (t3 * 10))),0,9);
		auto& number = lpImageMng.GetID(num, "number", Vector2I(25, 40), Vector2I(10, 1));
		DrawGraph(175, 10, number[t1], true);
		DrawGraph(200, 10, number[t2], true);
		DrawGraph(235, 10, number[t3], true);
		DrawGraph(260, 10, number[t4], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	}
}

EventType GmkEvent::GetNowEventType(void)
{
	return nowType_;
}


void GmkEvent::SetCollFunc(std::function<void(void)>&& startFunc, std::function<void(void)>&& endFunc)
{
	startCollFunc_.emplace_back(startFunc);
	endCollFunc_.emplace_back(endFunc);
}

void GmkEvent::UpdateAlert(float delta)
{
	if (sColor_ <= 1.0f && !scFlag_)
	{
		sColor_ += delta * 1.0f;
		if (sColor_ >= 1.0f)
		{
			scFlag_ = true;
		}
	}
	if (sColor_ >= 0.0f && scFlag_)
	{
		sColor_ -= delta * 1.0f;
		if (sColor_ <= 0.0f)
		{
			scFlag_ = false;
		}
	}
	if (gmkStepTime_ <= 0.0f)
	{
		for (auto& f : endCollFunc_)
		{
			f();
		}
		EndAlert();
		sColor_ = 0.0f;
		SoundProcess::SoundStop(SoundProcess::SOUNDNAME_BGM::alarm);
	}
}





