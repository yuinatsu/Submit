#include <DxLib.h>
#include "PlayerTutorial.h"
#include "../../Object/ObjManager.h"
#include "../../common/ImageMng.h"
#include "../../Input/InputConfig.h"
#include "../../common/GmkEvent.h"
#include "../../Object/Obj.h"
#include "../TutorialScene.h"

PlayerTutorial::PlayerTutorial(int mapNum)
{
	mapNum_ = mapNum;
	Init();
}

PlayerTutorial::~PlayerTutorial()
{
}

bool PlayerTutorial::UpdateTutorial(float delta)
{
	switch (TutorialType_)
	{
	case static_cast<int>(TUTORIAL_1::player):
		nowRate_ = 1;
		UpdateSwitching(0.1f);
		break;
	case static_cast<int>(TUTORIAL_1::target):
		UpdateSwitching(0.1f);
		break;
	case static_cast<int>(TUTORIAL_1::operation):
		UpdateSwitching(0.1f,true);
		break;
	case static_cast<int>(TUTORIAL_1::walk):
		UpdateSwitching(0.1f);
		break;
	case static_cast<int>(TUTORIAL_1::descriprion):
		UpdateSwitching(0.1f, true);
		break;
	case static_cast<int>(TUTORIAL_1::dashcrouch):
		if (count_ > 5.0f)
		{
			count_ = 0.0f;
			TutorialType_++;
			hitFlag_ = true;
		}
		break;
	case static_cast<int>(TUTORIAL_1::end):
		break;
	default:
		break;
	}
	return objMng_->UpdateTutorial1(delta, moveFlag_, hitFlag_);
}

void PlayerTutorial::DrawTutorial(float delta)
{
	objMng_->DrawTutorial1(camera_.GetOffset(), *drawMng_);
}

void PlayerTutorial::DrawSummary(float delta)
{
	if (!sumFlag_)
	{
		DrawRotaGraph(530, 330, nowRate_, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/TutorialPic.png")[0], true);
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
			nowRate_ = 1;
			DrawRotaGraph(900, 500, nowRate_, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/next0.png")[0], true);
		}
	}
}

void PlayerTutorial::DrawDescription(float delta)
{
	if (sumFlag_)
	{
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
		auto player = objMng_->GetPlayer();
		auto playerPos = static_cast<Vector2I>(player.lock()->GetPos());
		auto target = objMng_->GetTarget();
		auto targetPos = static_cast<Vector2I>(target.lock()->GetPos());
		switch (TutorialType_)
		{
		case static_cast<int>(TUTORIAL_1::player):
			DrawRotaGraph(playerPos.x + 48, playerPos.y + 25 + static_cast<int>(speed_), 0.8, Math::Deg2Rad(90.0), lpImageMng.GetID("./Resource/Image/TutorialPic/ya.png")[0], true);
			DrawRotaGraph(playerPos.x + 120, playerPos.y + 65, nowRate_, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial1-1.png")[0], true);
			break;
		case static_cast<int>(TUTORIAL_1::target):
			DrawRotaGraph(targetPos.x + 48, targetPos.y + 25 + static_cast<int>(speed_), 0.8, Math::Deg2Rad(90.0), lpImageMng.GetID("./Resource/Image/TutorialPic/ya.png")[0], true);
			DrawRotaGraph(targetPos.x + 120, targetPos.y + 65, nowRate_, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial1-2.png")[0], true);
			break;
		case static_cast<int>(TUTORIAL_1::operation):
			DrawRotaGraph(530, 330, nowRate_, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Player1.png")[0], true);
			if (controller_->GetCntType() == CntType::Key)
			{
				DrawRotaGraph(860, 500, 1, 0, keydata_[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
			}
			else
			{
				DrawRotaGraph(860, 500, 1, 0, paddata_[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
			}
			DrawRotaGraph(900, 500, 1, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/next0.png")[0], true);
			break;
		case static_cast<int>(TUTORIAL_1::walk):
			DrawRotaGraph(140, 510, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial1-3.png")[0], true);
			DrawRotaGraph(180, 550, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial1-4.png")[0], true);
			if (controller_->GetCntType() == CntType::Key)
			{
				DrawRotaGraph(80, 550, 1, 0, keydata_[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
			}
			else
			{
				DrawRotaGraph(80, 550, 1, 0, paddata_[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
			}
			break;
		case static_cast<int>(TUTORIAL_1::descriprion):
			DrawRotaGraph(530, 330, nowRate_, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Player2.png")[0], true);
			if (controller_->GetCntType() == CntType::Key)
			{
				DrawRotaGraph(860, 500, 1, 0, keydata_[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
			}
			else
			{
				DrawRotaGraph(860, 500, 1, 0, paddata_[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
			}
			DrawRotaGraph(900, 500, 1, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/next0.png")[0], true);
			break;
		case static_cast<int>(TUTORIAL_1::dashcrouch):
			DrawRotaGraph(230, 550, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial1-5.png")[0], true);
			break;
		case static_cast<int>(TUTORIAL_1::end):
			DrawRotaGraph(280, 530, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial1-6.png")[0], true);
			break;
		default:
			break;
		}
	}
}
