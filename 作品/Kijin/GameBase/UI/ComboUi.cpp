#include "ComboUi.h"
#include <DxLib.h>
#include "../Application.h"
#include "../Common/ResourceMng.h"
#include "../Common/Debug.h"
#include "../Object/ObjectManager.h"
#include "../Component/Behavior/PlayerBehavior.h"
#include "../Component/Info/ObjectInfo.h"

ComboUi::ComboUi(const std::filesystem::path& path, const Vector2& pos) : UiBase(pos)
{
	lpResourceMng.LoadTexture(handle_, path);
	pos_ = pos;
	//GetGraphSizeF(*handle_, &size_.x, &size_.y);
	auto div = Vector2I{ 10, 1 };
	auto size = Vector2I{ 100, 130 };
	LoadDivGraph(L"Resource/resource/Ui/num.png", div.x * div.y, div.x, div.y, size.x, size.y, hd_);
	combo_ = 0;
}

ComboUi::~ComboUi()
{
}

void ComboUi::Update(float delta, ObjectManager& objMng, Controller& controller)
{
	auto player = objMng.GetComponent<PlayerBehavior>(objMng.GetPlayerID());
	combo_ = player->GetComboNum();
}

void ComboUi::Draw()
{
	if (!combo_)
	{
		return;
	}

	auto size = Vector2{ 100, 130 };
	Vector2 comboFontPos = { pos_.x + 80, pos_.y + 40 };
	if (combo_ / 100 != 0)
	{
		// 3Œ…–Ú
		DrawGraph(pos_.x - size.x, pos_.y, hd_[combo_ / 100], true);
	}
	if (combo_ / 10 != 0)
	{
		// 2Œ…–Ú
		DrawGraph(pos_.x - size.x / 2, pos_.y, hd_[combo_ / 10], true);
	}
	// 1Œ…–Ú
	DrawGraph(pos_.x, pos_.y, hd_[combo_ % 10], true);
	DrawGraph(comboFontPos.x, comboFontPos.y, *handle_, true);
}
