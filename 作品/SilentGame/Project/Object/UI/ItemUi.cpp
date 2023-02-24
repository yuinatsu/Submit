#include <DxLib.h>
#include <algorithm>
#include "ItemUi.h"
#include "../../common/ImageMng.h"
#include "../../common/nameof.hpp"
#include "../../common/Debug.h"


ItemUi::ItemUi(const Math::Vector2& pos, const Math::Vector2& UISize) : UiBase(pos, UISize)
{
	pos_ = pos;
	UISize_ = UISize;
	// �A�C�e���T�C�Y
	size_ = { 32,32 };
	// �A�C�e�����g�p���
	useItem_ = ItemName::Max;
	// �}�X�NUI��
	mask_ = LoadMask("Resource/Image/UI/UIMask.png");
	// �A�C�e���A�C�R��
	lpImageMng.GetID("Resource/Image/UI/itemIcon.png", "Icon", size_, { 6,1 });
	// �A�C�e��ID�Z�b�g
	SetItemID(std::make_pair(0, "Knife"));

	updateFunc_ = std::bind(&ItemUi::UpdateStart, this, std::placeholders::_1, std::placeholders::_2);
	drawFunc_ = std::bind(&ItemUi::DrawStart, this, std::placeholders::_1);
	
	startFinItem_ = false;
	count_ = 0.0f;
	startBox_ = { 0.0 };
}

ItemUi::~ItemUi()
{
	order_.clear();
}

void ItemUi::Update(const double& delta, const std::shared_ptr<ObjManager> objMng)
{
	updateFunc_((float)delta, objMng);
}

void ItemUi::UpdateStart(const double& delta, const std::shared_ptr<ObjManager> objMng)
{
	controller_->Update(delta);
	count_ += (float)delta;
	// �o�ꎞ���o

	// �Q�[���J�n
	if (UISize_.x > startBox_.x + UISize_.x / 10.0f && count_ > 4.0f)
	{
		startBox_.x += UISize_.x / 20.0f;
	}
	else if (UISize_.x <= startBox_.x + UISize_.x / 10.0f)
	{
		startFinItem_ = true;
		updateFunc_ = std::bind(&ItemUi::UpdateGame, this, std::placeholders::_1, std::placeholders::_2);
		drawFunc_ = std::bind(&ItemUi::DrawGame, this, std::placeholders::_1);
	}
	if (!controller_->IsAnyPress())
	{
		startBox_.x = UISize_.x;
		startFinItem_ = true;
		updateFunc_ = std::bind(&ItemUi::UpdateGame, this, std::placeholders::_1, std::placeholders::_2);
		drawFunc_ = std::bind(&ItemUi::DrawGame, this, std::placeholders::_1);
	}
}

void ItemUi::UpdateGame(const double& delta, const std::shared_ptr<ObjManager> objMng)
{
	// �R���g���[���[���X�V
	controller_->Update(delta);
	// �A�C�e�����J�[�\��
	if (controller_->Pressed(InputID::ItemLeft))
	{
		// ����]
		if (order_.size())
		{
			std::rotate(order_.begin(), order_.begin() + (order_.size() - 1), order_.end());
		}
	}
	if (controller_->Pressed(InputID::ItemRight))
	{
		// �E��]
		if (order_.size())
		{
			std::rotate(order_.begin(), order_.begin() + 1, order_.end());
		}
	}
	// �U�����̃A�C�e��UI����
	auto tmp = controller_->LongPress(InputID::Attack, 3.0, delta);
	if (tmp == "Attack" && order_.size())
	{
		// �g�p�����A�C�e���̏��i�[
		useItem_ = static_cast<ItemName>(order_.at(0).first);
		if (useItem_ != ItemName::Knife && useItem_ != ItemName::Key)
		{
			// �擪�v�f�폜
			order_.erase(order_.begin());
		}
	}
	// �\�[�g
	for (int i = 0; i < order_.size(); i++)
	{
		bool isLead = false;
		// �擪�A�C�e��
		if (!i)
		{
			isLead = true;
		}
		order_.at(i).second = isLead;
	}

	objMng->SetHaveItem(order_);
}

void ItemUi::DrawUI(const double& delta)
{
	drawFunc_(delta);
}

void ItemUi::DrawStart(const double& delta)
{
	// �}�X�NUI�`��
	SetUseMaskScreenFlag(true);
	DrawMask(static_cast<int>(pos_.x), static_cast<int>(pos_.y), mask_, DX_MASKTRANS_BLACK);
	DrawBox(static_cast<int>(pos_.x), static_cast<int>(pos_.y), static_cast<int>(pos_.x + startBox_.x), static_cast<int>(pos_.y + UISize_.y), 0xffff00, true);
	SetUseMaskScreenFlag(false);
}

void ItemUi::DrawGame(const double& delta)
{
	// �}�X�NUI�`��
	SetUseMaskScreenFlag(true);
	DrawMask(static_cast<int>(pos_.x), static_cast<int>(pos_.y), mask_, DX_MASKTRANS_NONE);
	DrawBox(static_cast<int>(pos_.x), static_cast<int>(pos_.y), static_cast<int>(pos_.x + UISize_.x), static_cast<int>(pos_.y + UISize_.y), 0xffff00, true);
	SetUseMaskScreenFlag(false);
	// �A�C�e���`��
	DrawItem();
}

void ItemUi::DrawItem()
{
	auto pos = Vector2{ pos_.x + 25,pos_.y + 20 };
	for (int i = 0; i < order_.size(); i++)
	{
		// ���[�̃A�C�e��
		if (order_.at(i).second)
		{
			DrawExtendGraphF(pos.x, pos.y, pos.x + size_.x * 2, pos.y + size_.y * 2, lpImageMng.GetID("Icon")[order_.at(i).first], true);
		}
		// ���̑��A�C�e��
		else
		{
			DrawGraphF(pos.x + size_.x + size_.x * i, pos.y + size_.y, lpImageMng.GetID("Icon")[order_.at(i).first], true);
		}
	}
}
