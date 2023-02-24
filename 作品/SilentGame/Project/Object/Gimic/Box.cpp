#include "../../common/ImageMng.h"
#include "../../Component/Collider/CircleCollider.h"
#include "../../common/Structure.h"
#include "../ObjManager.h"
#include "../UI/ItemUi.h"
#include "Box.h"
#include "../../Resource/Tmx/TmxObj.h"
#include "../../common/DrawMng.h"
#include "../Unit/Enemy.h"
#include "../../common/MapCollMng.h"
#include "../../common/nameof.hpp"

constexpr char keyName[]{ "box" };


Box::Box(std::shared_ptr<TmxObj>& tmxObj,UiBase& itemUi,const Math::Vector2& pos, ObjManager& objMng, ItemName itemName) :
	objMng_{objMng}, itemName_{itemName}, itemUi_{itemUi}, Obj{pos} , tmxObj_{tmxObj}
{
	tmxObj_->SetNaviFlag(pos_, 15.0f, false);
	if (itemName_ == ItemName::Key)
	{
		imgKey_ = "KeyBox";
		lpImageMng.GetID("./Resource/Image/keyBox.png", imgKey_, Math::Vector2I{ 32,32 }, Math::Vector2I{ 2,1 });
		
	}
	else
	{
		imgKey_ = keyName;
		lpImageMng.GetID("./Resource/Image/box.png", imgKey_, Math::Vector2I{ 32,32 }, Math::Vector2I{ 2,1 });
	}
	Init();
}

Box::~Box()
{
}

void Box::Init(void)
{
	// �R���|�[�l���g��ǉ�
	AddComponent(std::make_unique<CircleCollider>(
		10.0f,
		CheckType::Box,
		std::bind(&Box::Hit, this, std::placeholders::_1),
		*this));
	// �����蔻����v���C���[�̖{�̂̂�����
	dynamic_cast<Collider&>(GetComponent(ComponentID::Collider)).SetTargetType(static_cast<int>(CheckType::PlayerBody) | static_cast<int>(CheckType::EnemyBody));
	flag_ = false;
	
}



void Box::Draw(const Vector2& drawoffset, DrawMng& drawMng)
{
	drawMng.Draw(pos_ + drawoffset, 0.0f, lpImageMng.GetID(imgKey_)[(flag_ == true ? 1:0)]);
	//DrawRotaGraph(static_cast<int>(pos_.x + drawoffset.x), static_cast<int>(pos_.y + drawoffset.y), 1.0, 0.0, lpImageMng.GetID((flag_ == true ? openImg : closeImg))[0], true);
}

bool Box::GetFlag(void)
{
	return flag_;
}

ItemName Box::GetItemName(void)
{
	return itemName_;
}

void Box::Hit(Collider& other)
{
	
	if (other.GetMyCheckType() == CheckType::EnemyBody)
	{
		if (dynamic_cast<Enemy&>(other.GetOwner()).GetEnemyState() == EnemyState::Gimic)
		{
			return;
		}
	}
	auto tmp = other.GetOwner().GetPos() - pos_;
	tmp.Normalize();
	auto length = dynamic_cast<CircleCollider&>(other).GetRadius() +
		dynamic_cast<CircleCollider&>(GetComponent(ComponentID::Collider)).GetRadius();
	
	
	// �����߂����ɕǂɂ߂荞�܂Ȃ����`�F�b�N����
	MapCollMng::Ray ray{ pos_ ,(tmp * length) };
	bool result = false;
	for (auto& c : tmxObj_->GetColList())
	{
		result |= MapCollMng::CheckCollision(ray, c).first;
		if (result)
		{
			// �߂荞�ގ����v�Ȉʒu�ɕ␳
			other.GetOwner().SetPos(
				pos_ + 
				((-other.GetOwner().GetMoveVec()) * (other.GetOwner().GetPos() - pos_).Magnitude())
			);
			break;
		}
	}

	if (!result)
	{
		// ���v�Ȏ������o��
		other.GetOwner().SetPos(pos_ + (tmp * length));
	}

	if (flag_)
	{
		return;
	}

	if (other.GetMyCheckType() == CheckType::PlayerBody)
	{
		std::string str(NAMEOF_ENUM(itemName_));
		if (itemUi_.SetItemID(std::make_pair(static_cast<int>(itemName_), str)))
		{
			flag_ = true;
			objMng_.AddEffect(EffectName::MagicSquare, pos_);
		}
	}
}


