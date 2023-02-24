#include <DxLib.h>
#include "DoorTutorial.h"
#include "../../common/ImageMng.h"
#include "../../Input/InputConfig.h"

DoorTutorial::DoorTutorial(int mapNum)
{
    mapNum_ = mapNum;
    Init();
}

DoorTutorial::~DoorTutorial()
{

}

bool DoorTutorial::UpdateTutorial(float delta)
{
	switch (TutorialType_)
	{
	case static_cast<int>(TUTORIAL_3::box):
		nowRate_ = 1;
		moveFlag_ = true;
		if (objMng_->GetBoxItem().at(ItemName::Key))
		{
			count_ = 0.0f;
			TutorialType_++;
			moveFlag_ = false;
			boxFlag_ = false;
		}
		break;
	case static_cast<int>(TUTORIAL_3::itember):
		UpdateSwitching(0.1f, true);
		break;
	case static_cast<int>(TUTORIAL_3::use):
		if (count_ >= 7.0f)
		{
			count_ = 0.0f;
			TutorialType_++;
			moveFlag_ = false;
			nowRate_ = 0.0f;
		}
		break;
	case static_cast<int>(TUTORIAL_3::alert):
		if (count_ >= 0.1f && controller_->Pressed(InputID::Attack))
		{
			count_ = 0.0f;
			TutorialType_++;
			moveFlag_ = true;
			AlertFlag_ = true;
		}
		break;
	case static_cast<int>(TUTORIAL_3::alerthit):
		if (objMng_->GetAlertFlag())
		{
			count_ = 0.0f;
			TutorialType_++;
		}
		break;
	case static_cast<int>(TUTORIAL_3::gmk):
		if (count_ >= 18)
		{
			count_ = 0.0f;
			TutorialType_++;
			hitFlag_ = true;
		}
		break;
	case static_cast<int>(TUTORIAL_3::end):
		break;
	default:
		break;
	}
	return objMng_->UpdateTutorial3(delta, moveFlag_, hitFlag_, AlertFlag_);
}

void DoorTutorial::DrawTutorial(float delta)
{
	objMng_->DrawTutorial3(camera_.GetOffset(), *drawMng_);
}

void DoorTutorial::DrawSummary(float delta)
{
	if (!sumFlag_)
	{
		DrawRotaGraph(530, 330, nowRate_, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Player4.png")[0], true);
		if (nowRate_ < 1)
		{
			nowRate_ += delta * 2;
		}
		else
		{
			if (controller_->GetCntType() == CntType::Key)
			{
				DrawRotaGraph(860, 550, 1, 0, keydata_[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
			}
			else
			{
				DrawRotaGraph(860, 550, 1, 0, paddata_[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
			}
			DrawRotaGraph(900, 550, 1, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/next0.png")[0], true);
			nowRate_ = 1;
		}
	}
}

void DoorTutorial::DrawSummary2(float delta)
{
	DrawRotaGraph(530, 330, nowRate_, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Player5.png")[0], true);
	if (nowRate_ < 1)
	{
		nowRate_ += delta * 2;
	}
	else
	{
		if (controller_->GetCntType() == CntType::Key)
		{
			DrawRotaGraph(860, 500, 1, 0, keydata_[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
		}
		else
		{
			DrawRotaGraph(860, 500, 1, 0, paddata_[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
		}
		DrawRotaGraph(900, 500, 1, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/next0.png")[0], true);
		nowRate_ = 1;
	}
}

void DoorTutorial::DrawDescription(float delta)
{
	if (sumFlag_)
	{
		switch (TutorialType_)
		{
		case static_cast<int>(TUTORIAL_3::box):
			if (!updownFalg_)
			{
				speed_ += delta * 20;
				if (speed_ > 10)
				{
					updownFalg_ = true;
				}
			}
			else
			{
				speed_ -= delta * 20;
				if (speed_ < 0)
				{
					updownFalg_ = false;
				}
			}
			DrawRotaGraph(250, 530, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial3-1.png")[0], true);
			DrawRotaGraph(353, 100 + static_cast<int>(speed_), 0.8, Math::Deg2Rad(90.0), lpImageMng.GetID("./Resource/Image/TutorialPic/ya.png")[0], true);
			break;
		case static_cast<int>(TUTORIAL_3::itember):
			DrawRotaGraph(200, 480, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial3-2.png")[0], true);
			DrawRotaGraph(150, 550, 0.8, Math::Deg2Rad(90.0), lpImageMng.GetID("./Resource/Image/TutorialPic/ya.png")[0], true);
			DrawRotaGraph(250, 550, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/next1.png")[0], true);
			if (controller_->GetCntType() == CntType::Key)
			{
				DrawRotaGraph(220, 550, 0.8, 0, keydata_[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
			}
			else
			{
				DrawRotaGraph(220, 550, 0.8, 0, paddata_[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
			}
			break;
		case static_cast<int>(TUTORIAL_3::use):
			DrawRotaGraph(200, 530, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial3-3.png")[0], true);
			break;
		case static_cast<int>(TUTORIAL_3::alert):
			DrawSummary2(delta);
			break;
		case static_cast<int>(TUTORIAL_3::alerthit):
			DrawRotaGraph(250, 560, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial3-6.png")[0], true);
			break;
		case static_cast<int>(TUTORIAL_3::gmk):
			DrawRotaGraph(280, 530, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial3-5.png")[0], true);
			break;
		case static_cast<int>(TUTORIAL_3::end):
			DrawRotaGraph(280, 530, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial3-4.png")[0], true);
			break;
		default:
			break;
		}
	}
}
