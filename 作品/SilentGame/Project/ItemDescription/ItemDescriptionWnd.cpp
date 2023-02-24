#include <DxLib.h>
#include "../common/MenuUI/MenuUICtrl.h"
#include "../Scene/SceneMng.h"
#include "../common/ImageMng.h"
#include "ItemDescriptionWnd.h"

constexpr float moveTimeMax_{ 0.5f };
constexpr Math::Vector2 pauseWndSizeHalf{ 780.0f/2.0f,340.0f/2.0f };

ItemDescriptionWnd::ItemDescriptionWnd(Controller& controller, const Math::Vector2& lt, const Math::Vector2& rb) :
	lt_{lt}, rb_{rb}, controller_{controller}
{
	// メニューを管理するクラスをセット
	menuUI_ = std::make_unique<MenuUICtrl<DescriptionID>>(controller_, static_cast<int>(DescriptionID::Clock) + 1, 3.0f, DescriptionID::Alcohol);

	// 左右でカーソルを操作する
	menuUI_->SetMinusInputID(InputID::Left);
	menuUI_->SetPlusInputID(InputID::Right);

	// メニューのついか
	menuUI_->AddMenu(
		DescriptionID::LandMine,
		std::make_unique<MenuUI>(Math::Vector2{ pauseWndSizeHalf.x - 160.0f, pauseWndSizeHalf.y }, "Icon", 3,2.0f)
	);
	menuUI_->AddMenu(
		DescriptionID::Decoy,
		std::make_unique<MenuUI>(Math::Vector2{ pauseWndSizeHalf.x - 64.0f, pauseWndSizeHalf.y}, "Icon", 1, 2.0f)
	);

	menuUI_->AddMenu(
		DescriptionID::Alcohol,
		std::make_unique<MenuUI>(Math::Vector2{ pauseWndSizeHalf.x + 64.0f, pauseWndSizeHalf.y }, "Icon", 2, 2.0f)
	);
	menuUI_->AddMenu(
		DescriptionID::Clock,
		std::make_unique<MenuUI>(Math::Vector2{ pauseWndSizeHalf.x + 160.0f, pauseWndSizeHalf.y }, "Icon", 4, 2.0f)
	);

	// 何も選択していない時の処理をセット
	draw_ = &ItemDescriptionWnd::DrawNon;
	update_ = &ItemDescriptionWnd::UpdateNon;
	flag_ = false;
}

bool ItemDescriptionWnd::Update(float delta)
{
	time_ += delta;
	return (this->*update_)(delta);
}

void ItemDescriptionWnd::Draw(void)
{
	(this->*draw_)();
}

bool ItemDescriptionWnd::GetFlag(void)
{
	return flag_;
}

bool ItemDescriptionWnd::UpdateNon(float delta)
{
	// メニューの更新
	menuUI_->Update(delta);

	if (controller_.Press(InputID::Attack) && time_ >= 0.5f)
	{
		flag_ = true;
		// アタックが押された時選択中のメニューにあったアイテムの表示を開始
		switch (menuUI_->GetNowMenu())
		{
			using enum DescriptionID;
		case LandMine:
			Start("loadPic_bomb");
			break;
		case Decoy:
			Start("laodPic_kakashi");
			break;
		case Alcohol:
			Start("loadPic_sake");
			break;
		case Clock:
			Start("loadPic_tokei");
			break;
		default:
			break;
		}
	}
	if (controller_.Press(InputID::btn1) && time_ >= 0.5f)
	{
		// 閉じる
		time_ = 0.0f;
		return false;
	}
	return true;
}

bool ItemDescriptionWnd::UpdateOpen(float delta)
{
	if (controller_.Pressed(InputID::btn1) && time_ >= moveTimeMax_)
	{
		flag_ = false;
		// btn1が押されていた時閉じる処理をセットする
		time_ = 0.0f;
		draw_ = &ItemDescriptionWnd::DrawClose;
		update_ = &ItemDescriptionWnd::UpdateClose;
	}
	return true;
}

bool ItemDescriptionWnd::UpdateClose(float delta)
{
	if (time_ >= moveTimeMax_)
	{
		// 移動演出用の時間値以上の時選択したいない処理をセットする
		draw_ = &ItemDescriptionWnd::DrawNon;
		update_ = &ItemDescriptionWnd::UpdateNon;
		time_ = 0.0f;
	}
	return true;
}

void ItemDescriptionWnd::DrawNon(void)
{
	menuUI_->Draw();

}

void ItemDescriptionWnd::DrawOpen(void)
{
	DrawNon();

	float val = Math::Clamp(time_ / moveTimeMax_,0.0f,1.0f);
	DrawBox(
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		0x000000,true
	);
	
	DrawModiGraph(
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		lpImageMng.GetID(imgName_)[0],true
	);
	DrawLine(
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		0xffffff, 5
	);
	DrawLine(
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		0xffffff, 5
	);
	DrawLine(
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		0xffffff, 5
	);
	DrawLine(
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		0xffffff, 5
	);
}

void ItemDescriptionWnd::DrawClose(void)
{
	DrawNon();

	float val = 1.0f - (time_ / moveTimeMax_);
	DrawBox(
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		0x000000, true
	);
	DrawModiGraph(
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		lpImageMng.GetID(imgName_)[0], true
	);
	DrawLine(
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		0xffffff, 5
	);
	DrawLine(
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		0xffffff, 5
	);
	DrawLine(
		static_cast<int>(pauseWndSizeHalf.x + imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		0xffffff, 5
	);
	DrawLine(
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y + imgSizeHalf_.y * val),
		static_cast<int>(pauseWndSizeHalf.x - imgSizeHalf_.x), static_cast<int>(pauseWndSizeHalf.y - imgSizeHalf_.y * val),
		0xffffff, 5
	);
}

void ItemDescriptionWnd::Start(const std::string& imgName)
{
	imgName_ = imgName;
	int x, y;
	GetGraphSize(lpImageMng.GetID(imgName)[0], &x, &y);
	imgSizeHalf_ = { static_cast<float>(x), static_cast<float>(y) };
	imgSizeHalf_ /= 2.0f;
	time_ = 0.0f;
	draw_ = &ItemDescriptionWnd::DrawOpen;
	update_ = &ItemDescriptionWnd::UpdateOpen;
}
