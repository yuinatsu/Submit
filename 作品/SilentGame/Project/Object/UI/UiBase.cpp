#include <DxLib.h>
#include <algorithm>
#include "UiBase.h"
#include "../../Input/Pad.h"
#include "../../Input/Keyboard.h"
#include "../../common/nameof.hpp"

UiBase::UiBase(const Math::Vector2& pos, const Math::Vector2& UISize) : pos_(pos), UISize_(UISize)
{
	// �R���g���[�����
	if (GetJoypadNum())
	{
		// �Q�[���p�b�h
		controller_ = std::make_unique<Pad>();
	}
	else
	{
		// �L�[�{�[�h
		controller_ = std::make_unique<Keyboard>();
	}
	// ������
	isAlive_ = true;
}

void UiBase::UpdateItem(const double& delta)
{
	// �����A�C�e�����ő�l�������甲����
	if (order_.size() >= haveItemMax)
	{
		return;
	}
	// �A�C�e��������
	for (auto& id : id_)
	{
		for (int n = 0; n < id_.size(); n++)
		{
			for (int i = 0; i < static_cast<int>(ItemName::Max); i++)
			{
				if (NAMEOF_ENUM(static_cast<ItemName>(i)) != id.second)
				{
					continue;
				}
				bool isLead = false;
				// �擪�A�C�e��
				if (!i)
				{
					isLead = true;
				}
				// �A�C�e���i�[
				order_.push_back(std::make_pair(i, isLead));
				break;
			}
			break;
		}
	}
	// �󂯎���Ă���id_���N���A
	id_.clear();
}

bool UiBase::SetItemID(IdData id)
{
	if (order_.size() < haveItemMax)
	{
		// �A�C�e���ǉ�
		id_.push_back(id);
		return true;
	}
	return false;
}

ItemName UiBase::GetUseItem()
{
	// �g�p���ꂽ�A�C�e�����i�[
	auto name = useItem_;
	// �A�C�e�����g�p��Ԃɖ߂�
	useItem_ = ItemName::Max;
	return name;
}
