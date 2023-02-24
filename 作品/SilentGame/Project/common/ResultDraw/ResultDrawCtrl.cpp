#include <DxLib.h>
#include "ResultDrawInt.h"
#include "ResultDrawLogo.h"
#include "../ResultData.h"
#include "../../Scene/SceneMng.h"
#include "ResultDrawFloat.h"
#include "../ImageMng.h"
#include "ResultDrawCtrl.h"
#include "../SoundPross.h"

constexpr float logoSizeY{ 100.0f };

ResultDrawCtrl::ResultDrawCtrl(ResultData& data, float delta) :
	time_{0.0f}
{
	lpImageMng.GetID("Resource/image/ResultScene/Numbers.png", "Numbers", Vector2I{ 50,50 }, Vector2I{ 3,4 });
	// ÉçÉSÇí«â¡
	if (data.IsClear())
	{ 
		drawList_.emplace_back(
			std::make_unique<ResultDrawLogo>(
				"Resource/image/ResultScene/GameClear.png",
				0.0f,
				Math::Vector2{ (static_cast<float>(lpSceneMng.GetScreenSize().x )/2.0f),logoSizeY },
				delta,
				true
		));
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_BGM::clear, SoundProcess::GetBGMVolumeEntire(), false);
	}
	else
	{
		drawList_.emplace_back(
			std::make_unique<ResultDrawLogo>(
				"Resource/image/ResultScene/GameOverLogo.png",
				0.0f,
				Math::Vector2{ (static_cast<float>(lpSceneMng.GetScreenSize().x) / 2.0f),logoSizeY },
				delta,
				false
		));

		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_BGM::fail, SoundProcess::GetBGMVolumeEntire(), false);
	}

	drawList_.emplace_back(
		std::make_unique<ResultDrawInt>(
			data.GetAlertCount(),
			"Resource/image/ResultScene/AlertCount.png",
			"Numbers",
			1.3f,
			Math::Vector2{ (static_cast<float>(lpSceneMng.GetScreenSize().x) / 2.0f),220.0f }
	));

	drawList_.emplace_back(
		std::make_unique<ResultDrawInt>(
			data.GetKillCount(),
			"Resource/image/ResultScene/KillCount.png",
			"Numbers",
			2.1f,
			Math::Vector2{ (static_cast<float>(lpSceneMng.GetScreenSize().x) / 2.0f),290.0f }
	));

	drawList_.emplace_back(
		std::make_unique<ResultDrawFloat>(
			data.GetClearTime(),
			"Resource/image/ResultScene/Time.png",
			"Numbers",
			2.9f,
			Math::Vector2{ (static_cast<float>(lpSceneMng.GetScreenSize().x) / 2.0f),360.0f }
	));


}

ResultDrawCtrl::~ResultDrawCtrl()
{
}

void ResultDrawCtrl::Update(float delta)
{
	time_ += delta;
	for (auto& d : drawList_)
	{
		d->Update(time_);
	}
}

void ResultDrawCtrl::Draw(void)
{
	for (auto& d : drawList_)
	{
		d->Draw();
	}
	/*DrawBox(
		(static_cast<float>(lpSceneMng.GetScreenSize().x) / 2.0f) - 350 ,50,
		(static_cast<float>(lpSceneMng.GetScreenSize().x) / 2.0f) + 350 ,400,
		0x00ff00,false
	);*/
}

const bool ResultDrawCtrl::IsStartedDraw(void) const
{
	bool rtn{ true };
	for (auto& d : drawList_)
	{
		rtn &= d->IsStartedDraw();
	}
	return rtn;
}
