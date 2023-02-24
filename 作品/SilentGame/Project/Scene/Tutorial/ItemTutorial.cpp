#include <DxLib.h>
#include "ItemTutorial.h"
#include "../../Object/ObjManager.h"
#include "../../common/ImageMng.h"
#include "../../Input/InputConfig.h"
#include "../../common/GmkEvent.h"
#include "../TutorialScene.h"

ItemTutorial::ItemTutorial(int mapNum)
{
    mapNum_ = mapNum;
    Init();
}

ItemTutorial::~ItemTutorial()
{
}

bool ItemTutorial::UpdateTutorial(float delta)
{
	switch (TutorialType_)
	{
	case static_cast<int>(TUTORIAL_2::itember):
		nowRate_ = 1;
		UpdateSwitching(0.1f);
		break;
	case static_cast<int>(TUTORIAL_2::knife):
		UpdateSwitching(0.1f, true);
		break;
	case static_cast<int>(TUTORIAL_2::box):
		if (objMng_->GetBoxItem().at(ItemName::LaundMine) != boxItem_.at(ItemName::LaundMine))
		{
			count_ = 0.0f;
			TutorialType_++;
			moveFlag_ = false;
			box3_ = false;
		}
		if (objMng_->GetBoxItem().at(ItemName::Alcohol) != boxItem_.at(ItemName::Alcohol))
		{
			count_ = 0.0f;
			TutorialType_++;
			moveFlag_ = false;
			box2_ = false;
		}
		if (objMng_->GetBoxItem().at(ItemName::Clock) != boxItem_.at(ItemName::Clock))
		{
			count_ = 0.0f;
			TutorialType_++;
			moveFlag_ = false;
			box4_ = false;
		}
		if (objMng_->GetBoxItem().at(ItemName::Decoy) != boxItem_.at(ItemName::Decoy))
		{
			count_ = 0.0f;
			TutorialType_++;
			moveFlag_ = false;
			box1_ = false;
		}
		if (boxItem_.at(ItemName::LaundMine) && boxItem_.at(ItemName::Alcohol) &&
			boxItem_.at(ItemName::Clock) && boxItem_.at(ItemName::Decoy))
		{
			count_ = 0.0f;
			TutorialType_ += 2;
			EnemyFlag_ = true;
		}
		break;
	case static_cast<int>(TUTORIAL_2::item):
		if (objMng_->GetBoxItem().at(ItemName::LaundMine) != boxItem_.at(ItemName::LaundMine))
		{
			if (controller_->Pressed(InputID::Attack) && count_ > 0.1f)
			{
				count_ = 0.0f;
				TutorialType_--;
				moveFlag_ = true;
				boxItem_.at(ItemName::LaundMine) = true;
			}
		}
		if (objMng_->GetBoxItem().at(ItemName::Alcohol) != boxItem_.at(ItemName::Alcohol))
		{
			if (controller_->Pressed(InputID::Attack) && count_ > 0.1f)
			{
				count_ = 0.0f;
				TutorialType_--;
				moveFlag_ = true;
				boxItem_.at(ItemName::Alcohol) = true;
			}
		}
		if (objMng_->GetBoxItem().at(ItemName::Clock) != boxItem_.at(ItemName::Clock))
		{
			if (controller_->Pressed(InputID::Attack) && count_ > 0.1f)
			{
				count_ = 0.0f;
				TutorialType_--;
				moveFlag_ = true;
				boxItem_.at(ItemName::Clock) = true;
			}
		}
		if (objMng_->GetBoxItem().at(ItemName::Decoy) != boxItem_.at(ItemName::Decoy))
		{
			if (controller_->Pressed(InputID::Attack) && count_ > 0.1f)
			{
				count_ = 0.0f;
				TutorialType_--;
				moveFlag_ = true;
				boxItem_.at(ItemName::Decoy) = true;
			}
		}
		break;
	case static_cast<int>(TUTORIAL_2::use):
		if (count_ >= 5.0f)
		{
			count_ = 0.0f;
			TutorialType_++;
			hitFlag_ = true;
		}
		break;
	case static_cast<int>(TUTORIAL_2::end):
		break;
	default:
		break;
	}
	return objMng_->UpdateTutorial2(delta, moveFlag_, hitFlag_,EnemyFlag_);
}

void ItemTutorial::DrawTutorial(float delta)
{
	objMng_->DrawTutorial2(camera_.GetOffset(), *drawMng_, EnemyFlag_);
}

void ItemTutorial::DrawSummary(float delta)
{
	if (!sumFlag_)
	{
		DrawRotaGraph(530, 330, nowRate_, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Player3.png")[0], true);
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
}

void ItemTutorial::DrawDescription(float delta)
{
	if (sumFlag_)
	{
		lpImageMng.GetID("./Resource/Image/TutorialPic/item.png", "item", Vector2I(475, 60), Vector2I(1, 4));
		auto& item = lpImageMng.GetID("item");
		switch (TutorialType_)
		{
		case static_cast<int>(TUTORIAL_2::itember):
			DrawRotaGraph(250, 480, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial2-1.png")[0], true);
			DrawRotaGraph(150, 550, 0.8, Math::Deg2Rad(90.0), lpImageMng.GetID("./Resource/Image/TutorialPic/ya.png")[0], true);
			DrawRotaGraph(250, 550, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/next1.png")[0], true);
			if (controller_->GetCntType() == CntType::Key)
			{
				DrawRotaGraph(220, 550, 1, 0, keydata_[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
			}
			else
			{
				DrawRotaGraph(220, 550, 1, 0, paddata_[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
			}
			break;
		case static_cast<int>(TUTORIAL_2::knife):
			DrawRotaGraph(250, 480, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial2-5.png")[0], true);
			DrawRotaGraph(250, 550, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/next1.png")[0], true);
			if (controller_->GetCntType() == CntType::Key)
			{
				DrawRotaGraph(220, 550, 1, 0, keydata_[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
			}
			else
			{
				DrawRotaGraph(220, 550, 1, 0, paddata_[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
			}
			break;
		case static_cast<int>(TUTORIAL_2::box):
			DrawRotaGraph(250, 500, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial2-2.png")[0], true);
			if (!updownFalg_)
			{
				speed_ += delta * 20;
				if (speed_ > 10)
				{
					updownFalg_ = true;
				}
			}else
			{
				speed_ -= delta * 20;
				if (speed_ < 0)
				{
					updownFalg_ = false;
				}
			}
			if (box1_)
			{
				DrawRotaGraph(box1X_, boxY_ + static_cast<int>(speed_), 0.8, Math::Deg2Rad(90.0), lpImageMng.GetID("./Resource/Image/TutorialPic/ya.png")[0], true);
			}
			if (box2_)
			{
				DrawRotaGraph(box2X_, boxY_ + static_cast<int>(speed_), 0.8, Math::Deg2Rad(90.0), lpImageMng.GetID("./Resource/Image/TutorialPic/ya.png")[0], true);
			}
			if (box3_)
			{
				DrawRotaGraph(box3X_, boxY_ + static_cast<int>(speed_), 0.8, Math::Deg2Rad(90.0), lpImageMng.GetID("./Resource/Image/TutorialPic/ya.png")[0], true);
			}
			if (box4_)
			{
				DrawRotaGraph(box4X_, boxY_ + static_cast<int>(speed_), 0.8, Math::Deg2Rad(90.0), lpImageMng.GetID("./Resource/Image/TutorialPic/ya.png")[0], true);
			}
			break;
		case static_cast<int>(TUTORIAL_2::item):
			if (objMng_->GetBoxItem().at(ItemName::LaundMine) != boxItem_.at(ItemName::LaundMine))
			{
				DrawGraph(ItemPosX_, ItemPosY_, item[3], true);
			}
			if (objMng_->GetBoxItem().at(ItemName::Alcohol) != boxItem_.at(ItemName::Alcohol))
			{
				DrawGraph(ItemPosX_, ItemPosY_, item[1], true);
			}
			if (objMng_->GetBoxItem().at(ItemName::Clock) != boxItem_.at(ItemName::Clock))
			{
				DrawGraph(ItemPosX_, ItemPosY_, item[2], true);
			}
			if (objMng_->GetBoxItem().at(ItemName::Decoy) != boxItem_.at(ItemName::Decoy))
			{
				DrawGraph(ItemPosX_, ItemPosY_, item[0], true);
			}
			break;
		case static_cast<int>(TUTORIAL_2::use):
			DrawRotaGraph(250, 500, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial2-3.png")[0], true);
			if (controller_->GetCntType() == CntType::Key)
			{
				DrawRotaGraph(70, 490, 1, 0, keydata_[lpConfigMng.GetKeyInputCode().at(InputID::ItemLeft)], true);
				DrawRotaGraph(90, 490, 1, 0, keydata_[lpConfigMng.GetKeyInputCode().at(InputID::ItemRight)], true);
				DrawRotaGraph(80, 520, 1, 0, keydata_[lpConfigMng.GetKeyInputCode().at(InputID::Attack)], true);
			}
			else
			{
				DrawRotaGraph(70, 490, 1, 0, paddata_[lpConfigMng.GetJoypadInputCode().at(InputID::ItemLeft)], true);
				DrawRotaGraph(90, 490, 1, 0, paddata_[lpConfigMng.GetJoypadInputCode().at(InputID::ItemRight)], true);
				DrawRotaGraph(80, 520, 1, 0, paddata_[lpConfigMng.GetJoypadInputCode().at(InputID::Attack)], true);
			}
			break;
		case static_cast<int>(TUTORIAL_2::end):
			DrawRotaGraph(280, 530, 0.8, 0, lpImageMng.GetID("./Resource/Image/TutorialPic/Tutorial2-4.png")[0], true);
			break;
		default:
			break;
		}
	}
}
