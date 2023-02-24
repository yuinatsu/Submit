#include "../../../Object/Unit/Enemy.h"
#include "../../Navi/Navi.h"
#include "../../Navi/PatrolNavi.h"
#include "../../Collider/FanCollider.h"
#include "ConditionNodeFunc.h"

bool CheckPosFunc::operator()(Obj& obj, float delta)
{
	// �͈͂��`�F�b�N
	return obj.GetPos().x > 0.0f && obj.GetPos().x < 1920.0f && obj.GetPos().y > 0.0f && obj.GetPos().y < 720.0f;
}

bool EnemyCheckAngleFunc::operator()(Obj& obj, float delta)
{
	auto& enemy = dynamic_cast<Enemy&>(obj);

	// �p�x���`�F�b�N����
	auto angle = Math::GetNormalizedAngle(Math::Square(obj.GetMoveVec().GetAngle() - enemy.targetAngle_));
	if (angle  <= Math::Square(enemy.rotSpeed_))
	{
		// �������l�ȉ��̎�true
		return true;
	}
	return false;
}



bool NaviStateCheckFindFunc::operator()(Obj& obj, float delta)
{
	// �i�r���擾
	auto& navi = dynamic_cast<Navi&>(obj.GetComponent(ComponentID::Navi));

	// �i�r�̏�Ԃ��ړI�n���������Ƃ�
	return navi.GetNaviState() == NaviState::Find;
}

bool NaviCheckDestFunc::operator()(Obj& obj, float delta)
{
	// �i�r���擾
	auto& navi = dynamic_cast<Navi&>(obj.GetComponent(ComponentID::Navi));

	// �i�r�̏�Ԃ��ړI�n�ɂ��������͉����Ȃ��Ƃ�
	return navi.GetNaviState() == NaviState::Non;
}


bool PatrolCheckNotIsStop::operator()(Obj& obj, float delta)
{
	// �i�r���擾
	auto& navi = dynamic_cast<PatrolBaseNavi&>(obj.GetComponent(ComponentID::PatrolNavi));

	// �i�r���X�g�b�v�ł͂Ȃ��Ƃ�true
	return !navi.IsStop();
}

bool FanColIsHitFunc::operator()(Obj& obj, float delta)
{
	// �t�@���^�̃R���C�_�[���擾
	auto& c = dynamic_cast<FanCollider&>(obj.GetComponent(ComponentID::Collider));
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return false;
	}
	return c.HitCheck(enemy->targetPos_);
}

bool IsMoveFunc::operator()(Obj& obj, float delta)
{
	// �ړ��ł��邩��Ԃ�
	return obj.IsMove(delta);
}


EnemyIsOverStateTimeFunc::EnemyIsOverStateTimeFunc(float limitTime) :
	limitTime_{limitTime}
{
}

bool EnemyIsOverStateTimeFunc::operator()(Obj& obj, float delta)
{
	// �G���擾
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return false;
	}
	// ���~�b�g�^�C�����z������true
	return enemy->GetStateTime() > limitTime_;
}


EnemyCheckStateFunc::EnemyCheckStateFunc(EnemyState state) :
	ckState_{state}
{
}

bool EnemyCheckStateFunc::operator()(Obj& obj, float delta)
{
	// �G���擾
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return false;
	}
	// �`�F�b�N����X�e�[�g�Ɠ�����������true
	return enemy->GetEnemyState() == ckState_;
}
