#include <DxLib.h>
#include <fstream>
#include "Unit/Player.h"
#include "Unit/Enemy.h"
#include "Gimic/SecurityCam.h"
#include "Gimic/Box.h"
#include "Gimic/TrapAlarm.h"
#include "Gimic/SnareFloor.h"
#include "Item/LaundMine.h"
#include "../Component/Collider/CircleCollider.h"
#include "../common/BehaviorTreeLoader.h"
#include "../Component/BehaviorTree/BehaviorTree.h"
#include "../Component/BehaviorTree/Node/BehaviorBaseNode.h"
#include "Unit/Target.h"
#include "Item/Decoy.h"
#include "Item/Alcohol.h"
#include "Item/Clock.h"
#include "../common/DrawMng.h"
#include "../common/GmkEvent.h"
#include "../common/nameof.hpp"
#include "Effect/Effect.h"
#include "ObjManager.h"
#include "Decoration.h"
#include "../common/ResultData.h"
#include "Footprints.h"
#include "Gimic/Door.h"
#include "../common/Debug.h"
#include "../common/SoundPross.h"

// �V�O�l�`��
constexpr char sig[4]{ 'd','a','t', 'a'};

// �o�[�W����
constexpr unsigned int version{ 5 };


ObjManager::ObjManager(
	const std::string& objDataName,
	std::shared_ptr< ResultData>& resultData,
	UiBase& itemUi,
	std::shared_ptr<GmkEvent>& gmkEvent,
	std::shared_ptr<TmxObj>& tmxObj,
	const Math::Vector2& scrSize) :
	gmkEvent_{ gmkEvent }, resultData_{resultData}
{
	Load(objDataName, itemUi, gmkEvent, tmxObj);
	CreateDoor(tmxObj);
	//Load("Resource/test.data", itemUi, gmkEvent, tmxObj);
	useItem_ = ItemName::Max;
	effect_ = EffectName::Max;
	pEnt_ = false;
	alertFlag_ = false;
	isClear_ = false;
}



ObjManager::~ObjManager()
{
	item_.clear();
}

bool ObjManager::Update(float delta)
{
	if (dynamic_cast<Target&>(*target_).IsClear())
	{
		isClear_ = true;
		return false;
	}

	if (!player_.get()->IsAlive())
	{
		return false;
	}

	if (!pEnt_)
	{
		AddEffect(EffectName_x120::Tornado, player_->GetPos());
		pEnt_ = true;
	}	
	player_->Update(delta);

	
	// �ړ��X�s�[�h�ω����̃G�t�F�N�g
	auto effect = player_->GetEffect();
	if (effect != effect_)
	{
		if (effect != EffectName::Non && effect != EffectName::Max)
		{
			AddEffect(effect, player_->GetPos());
		}
		effect_ = effect;
	}
	
	for (auto& enemy : enemyList_)
	{
		enemy->Update(delta);
	}
	target_->Update(delta);

	for (auto& gimic : gimicList_)
	{
		gimic->Update(delta);
	}
	
	UpdateEffect(delta);

	for (auto& item : itemList_)
	{
		item->Update(delta);
	}

	for (auto& d : doorList_)
	{
		d->Update(delta);
	}

	// �g�p���Ă���A�C�e����u������
	UseItem(useItem_);

	// �����蔻��̏���������
	CheckObjHit();

	// ���S�����L������j������(�폜�����������j���Ƃ��ĉ��Z)
	resultData_->AddKillCount(static_cast<int>(enemyList_.remove_if([](std::shared_ptr<Obj>& obj) {return (*obj).IsAlive() == false; })));

	// �g�p�����A�C�e����j������
	itemList_.remove_if([](std::unique_ptr<Obj>& obj) {return (*obj).IsAlive() == false; });

	return true;
}

void ObjManager::UpdateEffect(float delta)
{
	for (auto& effect : effectList_)
	{
		effect->Update(delta);
	}
	// �����ς݂̃G�t�F�N�g��j������
	effectList_.remove_if([](std::unique_ptr<Obj>& obj) {return (*obj).IsAlive() == false; });
}

bool ObjManager::UpdateTutorial1(float delta, bool moveFlag,bool hitFlag)
{
	if (dynamic_cast<Target&>(*target_).IsClear())
	{
		isClear_ = true;
		return false;
	}

	if (!player_.get()->IsAlive())
	{
		return false;
	}
	if (!pEnt_)
	{
		AddEffect(EffectName_x120::Tornado, player_->GetPos());
		pEnt_ = true;
	}
	if (moveFlag)
	{
		target_->Update(delta);
		player_->Update(delta);
	}


	// �ړ��X�s�[�h�ω����̃G�t�F�N�g
	auto effect = player_->GetEffect();
	if (effect != effect_)
	{
		if (effect != EffectName::Non && effect != EffectName::Max)
		{
			AddEffect(effect, player_->GetPos());
		}
		effect_ = effect;
	}


	for (auto& effect : effectList_)
	{
		effect->Update(delta);
	}

	// �����蔻��̏���������
	CheckObjHitTutorial1(hitFlag);


	// ���S�����L������j������(�폜�����������j���Ƃ��ĉ��Z)
	resultData_->AddKillCount(static_cast<int>(enemyList_.remove_if([](std::shared_ptr<Obj>& obj) {return (*obj).IsAlive() == false; })));
	// �g�p�����A�C�e����j������
	itemList_.remove_if([](std::unique_ptr<Obj>& obj) {return (*obj).IsAlive() == false; });
	// �����ς݂̃G�t�F�N�g��j������
	effectList_.remove_if([](std::unique_ptr<Obj>& obj) {return (*obj).IsAlive() == false; });

	return true;

}

bool ObjManager::UpdateTutorial2(float delta, bool moveFlag, bool hitFlag, bool enemyDrawFlag)
{
	if (dynamic_cast<Target&>(*target_).IsClear())
	{
		isClear_ = true;
		return false;
	}

	if (!player_.get()->IsAlive())
	{
		return false;
	}
	if (!pEnt_)
	{
		AddEffect(EffectName_x120::Tornado, player_->GetPos());
		pEnt_ = true;
	}
	if (moveFlag)
	{
		target_->Update(delta);
		player_->Update(delta);
	}


	// �ړ��X�s�[�h�ω����̃G�t�F�N�g
	auto effect = player_->GetEffect();
	if (effect != effect_)
	{
		if (effect != EffectName::Non && effect != EffectName::Max)
		{
			AddEffect(effect, player_->GetPos());
		}
		effect_ = effect;
	}

	for (auto& gimic : gimicList_)
	{
		gimic->Update(delta);
		
	}

	for (auto& item : itemList_)
	{
		item->Update(delta);
	}
	if (enemyDrawFlag)
	{
		for (auto& enemy : enemyList_)
		{
			enemy->Update(delta);
		}
		// �g�p���Ă���A�C�e����u������
		UseItem(useItem_);
	}

	for (auto& effect : effectList_)
	{
		effect->Update(delta);
	}

	// �����蔻��̏���������
	CheckObjHitTutorial2(hitFlag);

	// ���S�����L������j������(�폜�����������j���Ƃ��ĉ��Z)
	resultData_->AddKillCount(static_cast<int>(enemyList_.remove_if([](std::shared_ptr<Obj>& obj) {return (*obj).IsAlive() == false; })));
	// �g�p�����A�C�e����j������
	itemList_.remove_if([](std::unique_ptr<Obj>& obj) {return (*obj).IsAlive() == false; });
	// �����ς݂̃G�t�F�N�g��j������
	effectList_.remove_if([](std::unique_ptr<Obj>& obj) {return (*obj).IsAlive() == false; });

	return true;
}

bool ObjManager::UpdateTutorial3(float delta, bool moveFlag, bool hitFlag, bool gmcflag)
{
	if (dynamic_cast<Target&>(*target_).IsClear())
	{
		isClear_ = true;
		return false;
	}

	if (!player_.get()->IsAlive())
	{
		return false;
	}
	if (!pEnt_)
	{
		AddEffect(EffectName_x120::Tornado, player_->GetPos());
		pEnt_ = true;
	}
	if (moveFlag)
	{
		target_->Update(delta);
		player_->Update(delta);
	}


	// �ړ��X�s�[�h�ω����̃G�t�F�N�g
	auto effect = player_->GetEffect();
	if (effect != effect_)
	{
		if (effect != EffectName::Non && effect != EffectName::Max)
		{
			AddEffect(effect, player_->GetPos());
		}
		effect_ = effect;
	}
	for (auto& d : doorList_)
	{
		d->Update(delta);
	}


	for (auto& effect : effectList_)
	{
		effect->Update(delta);
	}

	// �����蔻��̏���������
	CheckObjHitTutorial3(hitFlag, gmcflag);

	HaveSpecItem(ItemName::Decoy);

	// ���S�����L������j������(�폜�����������j���Ƃ��ĉ��Z)
	resultData_->AddKillCount(static_cast<int>(enemyList_.remove_if([](std::shared_ptr<Obj>& obj) {return (*obj).IsAlive() == false; })));
	// �g�p�����A�C�e����j������
	itemList_.remove_if([](std::unique_ptr<Obj>& obj) {return (*obj).IsAlive() == false; });
	// �����ς݂̃G�t�F�N�g��j������
	effectList_.remove_if([](std::unique_ptr<Obj>& obj) {return (*obj).IsAlive() == false; });

	return true;
}

bool ObjManager::UpdateGameEnd(float delta)
{
	player_->Update(delta);

	for (auto& enemy : enemyList_)
	{
		enemy->Update(delta);
	}
	target_->Update(delta);

	for (auto& gimic : gimicList_)
	{
		gimic->Update(delta);
	}

	for (auto& item : itemList_)
	{
		item->Update(delta);
	}

	UpdateEffect(delta);

	return true;
}

void ObjManager::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{
	for (auto& footprint : footprintsList_)
	{
		footprint->Draw(offset);
	}

	for (auto& enemy : enemyList_)
	{
		enemy->Draw(offset, drawMng);
	}

	for (auto& gimic : gimicList_)
	{
		gimic->Draw(offset, drawMng);
	}

	for (auto& item : itemList_)
	{
		item->Draw(offset, drawMng);
	}

	if (pEnt_)
	{
		player_->Draw(offset, drawMng);
	}

	EffectDraw(offset, drawMng);

	target_->Draw(offset,drawMng);

	for (auto& d : doorList_)
	{
		d->Draw(offset,drawMng);
	}
}

void ObjManager::EffectDraw(const Math::Vector2& offset, DrawMng& drawMng)
{
	for (auto& effect : effectList_)
	{
		effect->Draw(offset, drawMng);
	}
}

void ObjManager::EffectDraw(DrawMng& drawMng)
{
	for (auto& effect : effectList_)
	{
		effect->Draw(Math::Vector2(0,0), drawMng);
	}
}

void ObjManager::DrawTutorial1(const Math::Vector2& offset, DrawMng& drawMng)
{
	for (auto& enemy : enemyList_)
	{
		enemy->Draw(offset, drawMng);
	}
	for (auto& gimic : gimicList_)
	{
		gimic->Draw(offset, drawMng);
	}
	for (auto& item : itemList_)
	{
		item->Draw(offset, drawMng);
	}

	player_->Draw(offset, drawMng);
	target_->Draw(offset, drawMng);

	EffectDraw(offset, drawMng);
}

void ObjManager::DrawTutorial2(const Math::Vector2& offset, DrawMng& drawMng, bool enemyFlag)
{
	if (enemyFlag)
	{
		for (auto& enemy : enemyList_)
		{
			enemy->Draw(offset, drawMng);
		}
	}
	for (auto& gimic : gimicList_)
	{
		gimic->Draw(offset, drawMng);
	}
	for (auto& item : itemList_)
	{
		item->Draw(offset, drawMng);
	}

	player_->Draw(offset, drawMng);
	target_->Draw(offset, drawMng);

	EffectDraw(offset, drawMng);
}

void ObjManager::DrawTutorial3(const Math::Vector2& offset, DrawMng& drawMng)
{
	for (auto& enemy : enemyList_)
	{
		enemy->Draw(offset, drawMng);
	}
	for (auto& gimic : gimicList_)
	{
		gimic->Draw(offset, drawMng);
	}
	for (auto& item : itemList_)
	{
		item->Draw(offset, drawMng);
	}

	player_->Draw(offset, drawMng);
	target_->Draw(offset, drawMng);

	EffectDraw(offset, drawMng);
	for (auto& d : doorList_)
	{
		d->Draw(offset, drawMng);
	}
}

void ObjManager::DrawGameEnd(const Math::Vector2& offset, DrawMng& drawMng)
{
	for (auto& enemy : enemyList_)
	{
		enemy->Draw(offset, drawMng);
	}

	for (auto& gimic : gimicList_)
	{
		gimic->Draw(offset, drawMng);
	}

	for (auto& item : itemList_)
	{
		item->Draw(offset, drawMng);
	}

	player_->Draw(offset, drawMng);

	EffectDraw(offset, drawMng);

	target_->Draw(offset, drawMng);
}

void ObjManager::UpdateStart(float delta)
{
	for (auto& enemy : enemyList_)
	{
		enemy->Update(delta);
	}
	target_->Update(delta);

}

std::weak_ptr<Obj> ObjManager::FindEnemy(const Math::Vector2& pos)
{
	enemyList_.sort([&pos](std::shared_ptr<Obj>& objA, std::shared_ptr<Obj>& objB) {
		
		return (objA->GetPos() - pos).SqMagnitude() < (objB->GetPos() - pos).SqMagnitude();
		}
	);
	return *enemyList_.begin();
}

void ObjManager::SortEnemyList(const Math::Vector2& pos)
{
	enemyList_.sort([&pos](std::shared_ptr<Obj>& objA, std::shared_ptr<Obj>& objB) {

		return (objA->GetPos() - pos).SqMagnitude() < (objB->GetPos() - pos).SqMagnitude();
		}
	);
}

std::list<std::shared_ptr<Obj>>& ObjManager::GetSortedEnemyList(const Math::Vector2& pos)
{
	SortEnemyList(pos);
	return enemyList_;
}

std::weak_ptr<Obj> ObjManager::GetPlayer(void)
{
	return player_;
}

BoxItem& ObjManager::GetBoxItem(void)
{
	return boxItem_;
}

std::weak_ptr<Obj> ObjManager::GetTarget(void)
{
	return target_;
}

std::weak_ptr<GmkEvent> ObjManager::GetGmkEvent(void)
{
	return gmkEvent_;
}

void ObjManager::UseItem(ItemName item)
{
	switch (item)
	{
	case ItemName::Knife:
		pEnt_ = true;
		return;
	case ItemName::Decoy:
		itemList_.emplace_back(std::make_unique<Decoy>(player_->GetPos()));
		AddEffect(EffectName::Light, player_->GetPos());
		resultData_->CountUpItemUseCount();
		break;
	case ItemName::Alcohol:
		itemList_.emplace_back(std::make_unique<Alcohol>(player_->GetPos()));
		AddEffect(EffectName::Light, player_->GetPos());
		resultData_->CountUpItemUseCount();
		break;
	case ItemName::LaundMine:
		itemList_.emplace_back(std::make_unique<LaundMine>(player_->GetPos()));
		AddEffect(EffectName::Light, player_->GetPos());
		resultData_->CountUpItemUseCount();
		break;
	case ItemName::Clock:
		itemList_.emplace_back(std::make_unique<Clock>(player_->GetPos(), *this));
		AddEffect(EffectName::Light, player_->GetPos());
		resultData_->CountUpItemUseCount();
		break;
	default:
		break;
	}
}

bool ObjManager::HaveSpecItem(ItemName item)
{
	for (int i = 0; i < item_.size(); i++)
	{
		if (item == static_cast<ItemName>(item_.at(i).first))
		{
			return true;
		}
	}
	return false;
}

void ObjManager::AddEffect(EffectName effectName, const Math::Vector2& pos)
{
	effectList_.emplace_back(std::make_unique<Effect>(effectName, pos));
}

void ObjManager::AddEffect(EffectName_x120 effectName, const Math::Vector2& pos)
{
	effectList_.emplace_back(std::make_unique<Effect>(effectName, pos));
}

void ObjManager::AddEffect(EffectNameWhole effectName, const Math::Vector2& pos)
{
	effectList_.emplace_back(std::make_unique<Effect>(effectName, pos));
}

bool ObjManager::GetAlertFlag(void)
{
	return alertFlag_;
}

void ObjManager::CheckObjHit(void)
{
	// �����蔻��
	// �v���C���[�̓����蔻��
	for (auto& pc : player_->GetAllComponent(ComponentID::Collider))
	{
		// �G�Ƃ̓����蔻��
		for (auto& enemy : enemyList_)
		{
			for (auto& ec : enemy->GetAllComponent(ComponentID::Collider))
			{
				dynamic_cast<Collider&>(*pc).HitCheck(dynamic_cast<Collider&>(*ec));
			}
		}
		// �M�~�b�N�Ƃ̓����蔻��
		for (auto& gimic : gimicList_)
		{
			// �R���C�_�[�������Ă��邩�`�F�b�N
			if (gimic->IncludeComponent(ComponentID::Collider))
			{
				// �����Ă���R���C�_�[�Ƃ̓����蔻����Ƃ�
				for (auto& gc : gimic->GetAllComponent(ComponentID::Collider))
				{
					dynamic_cast<Collider&>(*pc).HitCheck(dynamic_cast<Collider&>(*gc));
				}
			}
		}

		// �^�[�Q�b�g�Ƃ̔���
		for (auto& tc : target_->GetAllComponent(ComponentID::Collider))
		{
			dynamic_cast<Collider&>(*pc).HitCheck(dynamic_cast<Collider&>(*tc));
		}
	}

	// �M�~�b�N�ƓG�̔���
	for (auto& gimic : gimicList_)
	{
		// �����Ă���R���C�_�[�Ƃ̓����蔻����Ƃ�
		if (gimic->IncludeComponent(ComponentID::Collider))
		{
			for (auto& gc : gimic->GetAllComponent(ComponentID::Collider))
			{
				for (auto& enemy : enemyList_)
				{
					for (auto& ec : enemy->GetAllComponent(ComponentID::Collider))
					{
						dynamic_cast<Collider&>(*gc).HitCheck(dynamic_cast<Collider&>(*ec));
					}
				}
			}
		}
	}

	// �ݒu���ƓG�̔���
	for (auto& item : itemList_)
	{
		for (auto& enemy : enemyList_)
		{
			if (!item->IncludeComponent(ComponentID::Collider))
			{
				continue;
			}
			for (auto& ic : item->GetAllComponent(ComponentID::Collider))
			{
				for (auto& ec : enemy->GetAllComponent(ComponentID::Collider))
				{
					dynamic_cast<Collider&>(*ic).HitCheck(dynamic_cast<Collider&>(*ec));
				}
			}
		}
	}

	// �h�A�ƃv���C���[�̔���
	for (auto& door : doorList_)
	{
		for (auto& dcol : door->GetAllComponent(ComponentID::Collider))
		{
			dynamic_cast<Collider&>(player_->GetComponent(ComponentID::Collider)).HitCheck(dynamic_cast<Collider&>(*dcol));
		}
	}
}

void ObjManager::CheckObjHitTutorial1(bool hitflag)
{
	// �����蔻��
	// �v���C���[�̓����蔻��
	for (auto& pc : player_->GetAllComponent(ComponentID::Collider))
	{
		// �G�Ƃ̓����蔻��
		for (auto& enemy : enemyList_)
		{
			for (auto& ec : enemy->GetAllComponent(ComponentID::Collider))
			{
				dynamic_cast<Collider&>(*pc).HitCheck(dynamic_cast<Collider&>(*ec));
			}
		}
		// �M�~�b�N�Ƃ̓����蔻��
		for (auto& gimic : gimicList_)
		{
			// �R���C�_�[�������Ă��邩�`�F�b�N
			if (gimic->IncludeComponent(ComponentID::Collider))
			{
				// �����Ă���R���C�_�[�Ƃ̓����蔻����Ƃ�
				for (auto& gc : gimic->GetAllComponent(ComponentID::Collider))
				{
					dynamic_cast<Collider&>(*pc).HitCheck(dynamic_cast<Collider&>(*gc));
				}
			}
		}

		// �^�[�Q�b�g�Ƃ̔���
		if (hitflag)
		{
			for (auto& tc : target_->GetAllComponent(ComponentID::Collider))
			{
				dynamic_cast<Collider&>(*pc).HitCheck(dynamic_cast<Collider&>(*tc));
			}
		}
	}
}

void ObjManager::CheckObjHitTutorial2(bool hitflag)
{
	// �����蔻��
	// �v���C���[�̓����蔻��
	for (auto& pc : player_->GetAllComponent(ComponentID::Collider))
	{
		// �M�~�b�N�Ƃ̓����蔻��
		for (auto& gimic : gimicList_)
		{
			// �R���C�_�[�������Ă��邩�`�F�b�N
			if (gimic->IncludeComponent(ComponentID::Collider))
			{
				// �����Ă���R���C�_�[�Ƃ̓����蔻����Ƃ�
				for (auto& gc : gimic->GetAllComponent(ComponentID::Collider))
				{
					dynamic_cast<Collider&>(*pc).HitCheck(dynamic_cast<Collider&>(*gc));
					if (gimic->GetObjID() == ObjID::Box)
					{
						auto itemName = dynamic_cast<Box&>(*gimic).GetItemName();
						boxItem_[itemName] = dynamic_cast<Box&>(*gimic).GetFlag();
					}
				}
			}
		}
		if (hitflag)
		{
			// �^�[�Q�b�g�Ƃ̔���
			for (auto& tc : target_->GetAllComponent(ComponentID::Collider))
			{
				dynamic_cast<Collider&>(*pc).HitCheck(dynamic_cast<Collider&>(*tc));
			}
		}
	}

	// �M�~�b�N�ƓG�̔���
	for (auto& gimic : gimicList_)
	{
		// �����Ă���R���C�_�[�Ƃ̓����蔻����Ƃ�
		if (gimic->IncludeComponent(ComponentID::Collider))
		{
			for (auto& gc : gimic->GetAllComponent(ComponentID::Collider))
			{
				for (auto& enemy : enemyList_)
				{
					for (auto& ec : enemy->GetAllComponent(ComponentID::Collider))
					{
						dynamic_cast<Collider&>(*gc).HitCheck(dynamic_cast<Collider&>(*ec));
					}
				}
			}
		}
	}

	// �ݒu���ƓG�̔���
	for (auto& item : itemList_)
	{
		for (auto& enemy : enemyList_)
		{
			if (!item->IncludeComponent(ComponentID::Collider))
			{
				continue;
			}
			for (auto& ic : item->GetAllComponent(ComponentID::Collider))
			{
				for (auto& ec : enemy->GetAllComponent(ComponentID::Collider))
				{
					dynamic_cast<Collider&>(*ic).HitCheck(dynamic_cast<Collider&>(*ec));
				}
			}
		}
	}
}

void ObjManager::CheckObjHitTutorial3(bool hitflag , bool gmcFlag)
{
	// �����蔻��
// �v���C���[�̓����蔻��
	for (auto& pc : player_->GetAllComponent(ComponentID::Collider))
	{
		// �G�Ƃ̓����蔻��
		for (auto& enemy : enemyList_)
		{
			for (auto& ec : enemy->GetAllComponent(ComponentID::Collider))
			{
				dynamic_cast<Collider&>(*pc).HitCheck(dynamic_cast<Collider&>(*ec));
			}
		}
		// �M�~�b�N�Ƃ̓����蔻��
		for (auto& gimic : gimicList_)
		{
			// �R���C�_�[�������Ă��邩�`�F�b�N
			if (gimic->IncludeComponent(ComponentID::Collider))
			{
				// �����Ă���R���C�_�[�Ƃ̓����蔻����Ƃ�
				for (auto& gc : gimic->GetAllComponent(ComponentID::Collider))
				{
					if (gimic->GetObjID() == ObjID::Alert)
					{
						if (gmcFlag)
						{
							dynamic_cast<Collider&>(*pc).HitCheck(dynamic_cast<Collider&>(*gc));
							alertFlag_ = dynamic_cast<Alert&>(*gimic).GetpFlag();
						}
					}
					else
					{
						dynamic_cast<Collider&>(*pc).HitCheck(dynamic_cast<Collider&>(*gc));
						if (gimic->GetObjID() == ObjID::Box)
						{
							auto itemName = dynamic_cast<Box&>(*gimic).GetItemName();
							boxItem_[itemName] = dynamic_cast<Box&>(*gimic).GetFlag();
						}
					}
				}
			}
		}
		// �^�[�Q�b�g�Ƃ̔���
		if (hitflag)
		{
			for (auto& tc : target_->GetAllComponent(ComponentID::Collider))
			{
				dynamic_cast<Collider&>(*pc).HitCheck(dynamic_cast<Collider&>(*tc));
			}
		}
	}
	// �h�A�ƃv���C���[�̔���
	for (auto& door : doorList_)
	{
		for (auto& dcol : door->GetAllComponent(ComponentID::Collider))
		{
			dynamic_cast<Collider&>(player_->GetComponent(ComponentID::Collider)).HitCheck(dynamic_cast<Collider&>(*dcol));
		}
	}
}

bool ObjManager::Load(const std::string& xmlName, UiBase& itemUi, std::shared_ptr<GmkEvent>& gmkEvent, std::shared_ptr<TmxObj>& tmxObj)
{
	// �z�u�f�[�^��ǂݍ���
	std::ifstream ifs(xmlName, std::ios::binary);

	// �t�@�C���I�[�v���������`�F�b�N
	if (!ifs)
	{
		// ���ĂȂ��Ƃ���������߂�
		return false;
	}

	// �t�@�C���w�b�_�[
	Header h;

	// �w�b�_�[����ǂݍ���
	ifs.read(reinterpret_cast<char*>(&h), sizeof(h));

	if (h.ver != version)
	{
		DebugLog("���̃o�[�W�����ɑΉ����Ă��܂���");
		return false;
	}

	// �V�O�l�`�����`�F�b�N
	for (int i = 0; i < 4; i++)
	{
		if (h.s[i] != sig[i])
		{
			return false;
		}
	}

	// �r�w�C�r�A�c���[��ǂݍ���
	std::unique_ptr<BehaviorTreeLoader> bloader = std::make_unique<BehaviorTreeLoader>();

	const std::unordered_map<ObjID, std::function<void(ObjData&, std::ifstream&)>> func{
		{ObjID::Player, [this,&tmxObj](ObjData& objData, std::ifstream& f) {
			CreatePlayer(tmxObj,objData,f);
		}},
		{ObjID::Enemy,[this,&tmxObj,&bloader](ObjData& objData, std::ifstream& f) {
			CreateEnemy(*bloader,tmxObj,objData,f);
		}},
		{ObjID::Box, [this,&tmxObj,&itemUi](ObjData& objData, std::ifstream& f) {
			// �{�b�N�X�𐶐�����
			int item = 0;		// �A�C�e���̎��
			f.read(reinterpret_cast<char*>(&item), sizeof(item));
			auto& g = gimicList_.emplace_back(std::make_unique<Box>(tmxObj, itemUi,objData.pos_, *this, static_cast<ItemName>(item)));
			boxItem_.emplace(static_cast<ItemName>(item), false);
		}},
		{ObjID::Alert, [this,&tmxObj](ObjData& objData, std::ifstream& f) {
			// �Z�L�����e�B�J����
			// objData.pos�ɍ��W objData.vec�Ƀx�N�g���������Ă�
			gimicList_.emplace_back(std::make_unique<Alert>(gmkEvent_,objData.pos_,objData.vec_,tmxObj,player_));
		}},
		{ObjID::TrapAlarm, [this](ObjData& objData, std::ifstream& f) {
			auto& g = gimicList_.emplace_back(std::make_unique<TrapAlarm>(objData.pos_,gmkEvent_));
		}},
		{ObjID::SnareFloor, [this](ObjData& objData, std::ifstream& f) {
		}},
		{ObjID::Target, [this,&tmxObj,&bloader](ObjData& objData, std::ifstream& f) {
			CreateTarget(*bloader,tmxObj,objData,f);
		}},
		{ObjID::Decoration, [this,&tmxObj](ObjData& objData, std::ifstream& f) {
			CreateDecoration(tmxObj,objData,f);
		}}
	};
	// �f�[�^�̃w�b�_�[���
	DateHeader dh;

	// �ǂݍ���
	ifs.read(reinterpret_cast<char*>(&dh), sizeof(dh));
	int sum = 0;
	// �t�@�C����EOF�̈ʒu�܂Ń��[�v
	while (!ifs.eof())
	{
		// �f�[�^�̌���for������
		for (int i = 0; i < dh.num; i++)
		{
			// �I�u�W�F�N�g�p�̃f�[�^��ǂ�
			ObjData objData;
			int strSize;
			ifs.read(reinterpret_cast<char*>(&objData.pos_), sizeof(objData.pos_));
			ifs.read(reinterpret_cast<char*>(&objData.vec_), sizeof(objData.vec_));
			ifs.read(reinterpret_cast<char*>(&strSize), sizeof(strSize));
			objData.imgName_.resize(strSize);
			ifs.read(reinterpret_cast<char*>(objData.imgName_.data()), sizeof(objData.imgName_[0]) * strSize);

			// ��ނɍ��킹�ăI�u�W�F�N�g�𐶐�����
			func.at(dh.id)(objData, ifs);
		}
		sum += dh.num;
		ifs.read(reinterpret_cast<char*>(&dh), sizeof(dh));
	}

	if (h.sum != sum)
	{
		DebugLog("�f�[�^������������܂���");
		return false;
	}

	return true;
}

void ObjManager::CreatePlayer(std::shared_ptr<TmxObj>& tmxObj,ObjManager::ObjData& obj, std::ifstream& f)
{
	// �R���g���[���[���
	if (GetJoypadNum())
	{
		player_ = std::make_unique<Player>(CntType::Pad, tmxObj, obj.pos_);
	}
	else
	{
		player_ = std::make_unique<Player>(CntType::Key, tmxObj, obj.pos_);
	}

	// �{�̂̓����蔻���ǉ�
	player_->AddComponent(std::make_unique<CircleCollider>(12.0f, CheckType::PlayerBody, [](Collider& other) {}, *player_));
	
	dynamic_cast<Collider&>(player_->GetComponent(ComponentID::Collider, 0)).SetTargetType(
		static_cast<int>(CheckType::Box) | 
		static_cast<int>(CheckType::TrapAlarm) | 
		static_cast<int>(CheckType::SnareFloor) | 
		static_cast<int>(CheckType::EnemyBody) | 
		static_cast<int>(CheckType::EnemyFan)| 
		static_cast<int>(CheckType::Door));

	// �U���p�̓����蔻���ǉ�
	player_->AddComponent(std::make_unique<CircleCollider>(24.0f, CheckType::PlayerAttack, [this](Collider& other) {
		if (!player_->IsAttack() && useItem_ == ItemName::Knife && other.GetMyCheckType() == CheckType::EnemyBody)
		{
			player_->SetAttackFlag(true);
			// �a���G�t�F�N�g
			AddEffect(EffectName::Slash, other.GetOwner().GetPos());
			SoundProcess::PlayBackSound(SoundProcess::SOUNDNAME_SE::slash, SoundProcess::GetSEVolumeEntire(), false);
			other.GetOwner().Destroy();
		}
	}, *player_));

	// �����̓����蔻���ǉ�
	player_->AddComponent(std::make_unique<CircleCollider>(12.0f, CheckType::Sound, [](Collider& other) {}, *player_));
	dynamic_cast<Collider&>(player_->GetComponent(ComponentID::Collider, 2)).SetTargetType(static_cast<int>(CheckType::EnemySound));
}

void ObjManager::CreateEnemy(BehaviorTreeLoader& bloder, std::shared_ptr<TmxObj>& tmxObj, ObjData& obj, std::ifstream& f)
{
	int num;
	std::vector<Math::Vector2I> route;
	// ���񃋁[�g�ǂݍ���
	f.read(reinterpret_cast<char*>(&num), sizeof(num));
	route.resize(num);
	f.read(reinterpret_cast<char*>(route.data()), sizeof(route[0]) * num);

	// �r�w�C�r�A�c���[�̃f�[�^�̃t�@�C������ǂݍ���
	std::string xmlName;
	f.read(reinterpret_cast<char*>(&num), sizeof(num));
	xmlName.resize(num);
	f.read(reinterpret_cast<char*>(xmlName.data()), sizeof(xmlName[0]) * num);
	auto& e = enemyList_.emplace_back(std::make_shared<Enemy>(*this,gmkEvent_, route, tmxObj, obj.pos_));

	footprintsList_.push_back(std::make_unique<Footprints>(route, tmxObj));

	// �R���|�[�l���g�ǉ�
	e->AddComponent(std::make_unique<BehaviorTree>(std::move(bloder.Load(xmlName)), *e));
}

void ObjManager::CreateTarget(BehaviorTreeLoader& bloder, std::shared_ptr<TmxObj>& tmxObj, ObjData& obj, std::ifstream& f)
{
	int num;
	std::vector<Math::Vector2I> route;
	// ���񃋁[�g�ǂݍ���
	f.read(reinterpret_cast<char*>(&num), sizeof(num));
	route.resize(num);
	f.read(reinterpret_cast<char*>(route.data()), sizeof(route[0]) * num);

	// �r�w�C�r�A�c���[�̃f�[�^�̃t�@�C������ǂݍ���
	std::string xmlName;
	f.read(reinterpret_cast<char*>(&num), sizeof(num));
	xmlName.resize(num);
	f.read(reinterpret_cast<char*>(xmlName.data()), sizeof(xmlName[0]) * num);
	target_ = std::make_shared<Target>(route,tmxObj,obj.pos_);

	target_->AddComponent(std::make_unique<BehaviorTree>(std::move(bloder.Load(xmlName)), *target_));

	// �R���|�[�l���g�ǉ�
	// e->AddComponent(std::make_unique<BehaviorTree>(std::move(bloder.Load(xmlName)), *e));
}

void ObjManager::CreateDecoration(std::shared_ptr<TmxObj>& tmxObj, ObjData& obj, std::ifstream& f)
{
	bool isAnim;
	bool isCol;
	float r;
	f.read(reinterpret_cast<char*>(&isCol), sizeof(isCol));

	// �����蔻�����邩���`�F�b�N
	if (isCol)
	{
		// ��鎞���a���擾
		f.read(reinterpret_cast<char*>(&r), sizeof(r));
	}

	f.read(reinterpret_cast<char*>(&isAnim), sizeof(isAnim));
	if (isAnim)
	{
		// �A�j���[�V��������Ƃ�
		std::string animFname;
		std::string state;
		std::string key;
		int size;
		f.read(reinterpret_cast<char*>(&size), sizeof(size));
		animFname.resize(size);
		f.read(reinterpret_cast<char*>(animFname.data()), sizeof(animFname[0]) * size);
		f.read(reinterpret_cast<char*>(&size), sizeof(size));
		state.resize(size);
		f.read(reinterpret_cast<char*>(state.data()), sizeof(state[0] )* size);
		f.read(reinterpret_cast<char*>(&size), sizeof(size));
		key.resize(size);
		f.read(reinterpret_cast<char*>(key.data()), sizeof(key[0] )* size);

		if (isCol)
		{
			// �����蔻�肠�鎞
			tmxObj->SetNaviFlag(obj.pos_,r, false);
			gimicList_.emplace_back(std::make_unique<Decoration>(animFname, state, key, tmxObj,r, obj.pos_));
		}
		else
		{
			// �Ȃ��Ƃ�
			gimicList_.emplace_back(std::make_unique<Decoration>(animFname, state, key, obj.pos_));
		}
	}
	else
	{
		// �A�j���[�V�������Ȃ��Ƃ�
		std::string imgFname;
		std::string key;
		Math::Vector2I divSize;
		Math::Vector2I divNum;
		int idx;
		int size;
		f.read(reinterpret_cast<char*>(&size), sizeof(size));
		imgFname.resize(size);
		f.read(reinterpret_cast<char*>(imgFname.data()), sizeof(imgFname[0]) * size);
		f.read(reinterpret_cast<char*>(&size), sizeof(size));
		key.resize(size);
		f.read(reinterpret_cast<char*>(key.data()), sizeof(key[0]) * size);
		f.read(reinterpret_cast<char*>(&divSize), sizeof(divSize));
		f.read(reinterpret_cast<char*>(&divNum), sizeof(divNum));
		f.read(reinterpret_cast<char*>(&idx), sizeof(idx));

		if (isCol)
		{
			// �����蔻�肠��Ƃ�
			tmxObj->SetNaviFlag(obj.pos_,r, false);
			gimicList_.emplace_back(std::make_unique<Decoration>(imgFname, key, divSize, divNum, idx,tmxObj, r, obj.pos_));
		}
		else
		{
			// �Ȃ��Ƃ�
			gimicList_.emplace_back(std::make_unique<Decoration>(imgFname, key, divSize, divNum, idx, obj.pos_));
		}
	}


}

void ObjManager::CreateDoor(std::shared_ptr<TmxObj>& tmx)
{
	for (auto& door : tmx->GetDoorData())
	{
		doorList_.emplace_back(std::make_unique<Door>(door.first,*this,gmkEvent_,tmx, door.second));
	}
	tmx->GetDoorData().clear();
}
