#include "../SoundPross.h"
#include "../../Input/Controller.h"
#include "../MenuUI/MenuUICtrl.h"
#include "../ImageMng.h"
#include "../../Input/InputConfigMng.h"
#include "../../Input/InputConfig.h"
#include "SoundOption.h"

SoundOption::SoundOption(Controller& controller) :
	OptionWindow{ controller }
{

	menuUICtrl_ = std::make_unique<MenuUICtrl<SOUNDSELECT>>(controller_, static_cast<int>(SOUNDSELECT::Max), 3.0f, SOUNDSELECT::Bgm);

	menuUICtrl_->AddMenu(
		SOUNDSELECT::Bgm,
		std::make_unique<MenuUI>(
			Math::Vector2{ 500.0f,212.5f },
			"Resource/Image/Option/BGM.png"
			));

	menuUICtrl_->AddMenu(
		SOUNDSELECT::Se,
		std::make_unique<MenuUI>(
			Math::Vector2{ 500.0f,272.5f },
			"Resource/Image/Option/SE.png"
			));

	menuUICtrl_->AddMenu(
		SOUNDSELECT::Exit,
		std::make_unique<MenuUI>(
			Math::Vector2{ 500.0f,332.5f },
			"Resource/Input/Exit.png"
			));
	auto type = GetJoypadType(DX_INPUT_PAD1);
	lpImageMng.GetID("./Resource/Input/PaddataPS.png", "PadPS", Vector2I(90, 25), Vector2I(10, 2));
	lpImageMng.GetID("./Resource/Input/Paddata.png", "PadXbox", Vector2I(75, 25), Vector2I(10, 1));
	VolumeType_ = 0;
	updata_ = std::bind(&SoundOption::UpdateSoundSet, this, std::placeholders::_1);
	draw_ = std::bind(&SoundOption::DrawSoundSet, this);
}
bool SoundOption::Update(float delta)
{
	time_ += delta;
	return updata_(delta);
}

void SoundOption::Draw(void)
{
	draw_();
}

bool SoundOption::UpdateSoundSet(float delta)
{
	menuUICtrl_->Update(delta);
	if (controller_.Pressed(InputID::Attack) && time_ > 0.2f)
	{
		if (menuUICtrl_->GetNowMenu() == SOUNDSELECT::Se)
		{
			updata_ = std::bind(&SoundOption::UpdateSEset, this, std::placeholders::_1);
			draw_ = std::bind(&SoundOption::DrawSet, this);
			VolumeType_ = static_cast<int>(SOUNDSELECT::Se) + 1;
			isDrawInput_ = false;
			time_ = 0.0f;
		}
		else if (menuUICtrl_->GetNowMenu() == SOUNDSELECT::Bgm)
		{
			updata_ = std::bind(&SoundOption::UpdateBGset, this, std::placeholders::_1);
			draw_ = std::bind(&SoundOption::DrawSet, this);
			VolumeType_ = static_cast<int>(SOUNDSELECT::Bgm) + 1;
			time_ = 0.0f;
			isDrawInput_ = false;
		}
		else
		{
			return false;
		}
	}
	return true;
}

void SoundOption::DrawSoundSet(void)
{
	menuUICtrl_->Draw();
	int bgVal = static_cast<int>(SoundProcess::GetBGMVolumeEntire() * 10.0f);
	int seVal = static_cast<int>(SoundProcess::GetSEVolumeEntire() * 10.0f);
	// まれにtが10になるので応急処置でクランプしてます
	int t1 = std::clamp(static_cast<int>(bgVal / 10), 0, 9);
	int t2 = std::clamp(static_cast<int>(bgVal - (t1 * 10)), 0, 9);
	//int tmp = (bgVal / 10) % 10;
	if (VolumeType_ == 1)
	{
		SetDrawBright(255, 0, 0);
	}
	DrawGraph(570, 200, lpImageMng.GetID("volumeNumber")[t1], true);
	DrawGraph(585, 200, lpImageMng.GetID("volumeNumber")[t2], true);
	SetDrawBright(255, 255,255 );
	DrawBox(500, 227, 800, 252, 0xffffff, false);
	DrawBox(505, 232, 505 + static_cast<int>(((t1*10)+ t2)* 29.0f), 247, 0xffffff, true);

	t1 = std::clamp(static_cast<int>(seVal / 10), 0, 9);
	t2 = std::clamp(static_cast<int>(seVal - (t1 * 10)), 0, 9);
	if (VolumeType_ == 2)
	{
		SetDrawBright(255, 0, 0);
	}
	DrawGraph(570, 260, lpImageMng.GetID("volumeNumber")[t1], true);
	DrawGraph(585, 260, lpImageMng.GetID("volumeNumber")[t2], true);
	SetDrawBright(255, 255, 255);
	DrawBox(500, 285, 800, 310, 0xffffff, false);
	DrawBox(505, 290, 505 + static_cast<int>(((t1 * 10) + t2) * 29.0f), 305, 0xffffff, true);


	
}

bool SoundOption::UpdateSEset(float delta)
{
	float val = SoundProcess::GetSEVolumeEntire();
	if (controller_.Pressed(InputID::Left))
	{
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select, SoundProcess::GetSEVolumeEntire(), false);
		//val -= 0.09f;
		val -= 0.1f;
		val = (val * 100 + 1) / 100;
	}
	if (controller_.Pressed(InputID::Right))
	{
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select, SoundProcess::GetSEVolumeEntire(), false);
		//val += 0.09f;
		val += 0.1f;
		val = (val * 100 + 1) / 100;
	}
	if (controller_.Pressed(InputID::Attack) && time_ > 0.2f)
	{
		SoundProcess::SetSEVolumeEntire(0.0f);
		updata_ = std::bind(&SoundOption::UpdateSoundSet, this, std::placeholders::_1);
		draw_ = std::bind(&SoundOption::DrawSoundSet, this);
		VolumeType_ = 0;
		isDrawInput_ = true;
	}
	SoundProcess::SetSEVolumeEntire(std::clamp(val,0.0f,1.0f));
	
	return true;
}

void SoundOption::DrawSet(void)
{
	DrawSoundSet();
	auto& keydata = lpImageMng.GetID("Keydata");
	auto type = GetJoypadType(DX_INPUT_PAD1);
	
	auto cntType = controller_.GetCntType();

	DrawGraph(500, 425, lpImageMng.GetID("./Resource/Image/Option/setting.png")[0], true);
	if (cntType == CntType::Key)
	{
		DrawGraph(775, 425, lpImageMng.GetID("./Resource/Image/Option/decision.png")[0], true);
		DrawGraph(850, 425, keydata[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
	}
	else
	{
		std::string tmpKeyStr;
		if (type == 3 || type == 4)
		{
			tmpKeyStr = "PadPS";
		}
		else
		{
			tmpKeyStr = "PadXbox";
		}
		auto& paddata = lpImageMng.GetID(tmpKeyStr);
		//DrawFormatString(775, 425, 0xffffff, "戻る：");
		DrawGraph(775, 425, lpImageMng.GetID("./Resource/Image/Option/decision.png")[0], true);
		DrawGraph(850, 425, paddata[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
	}
}

bool SoundOption::UpdateBGset(float delta)
{
	float val = SoundProcess::GetBGMVolumeEntire();
	if (controller_.Pressed(InputID::Left))
	{
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select, SoundProcess::GetSEVolumeEntire(), false);
		//val -= 0.09f;
		val -= 0.1f;
		val = (val * 100 + 1) / 100;
	}
	if (controller_.Pressed(InputID::Right))
	{
		SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::select, SoundProcess::GetSEVolumeEntire(), false);
		//val += 0.09f;
		val += 0.1f;
		val = (val * 100 + 1) / 100;
	}
	if (controller_.Pressed(InputID::Attack) && time_ > 0.2f)
	{
		SoundProcess::SetBGMVolumeEntire(0.0f);
		updata_ = std::bind(&SoundOption::UpdateSoundSet, this, std::placeholders::_1);
		draw_ = std::bind(&SoundOption::DrawSoundSet, this);
		isDrawInput_ = true;
		VolumeType_ = 0;
	}
	SoundProcess::SetBGMVolumeEntire(std::clamp(val, 0.0f, 1.0f));
	return true;
}

