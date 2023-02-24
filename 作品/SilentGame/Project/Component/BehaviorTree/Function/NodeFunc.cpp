#include "../../../Object/Unit/Enemy.h"
#include "../../Navi/Navi.h"
#include "../../Navi/PatrolNavi.h"
#include "../../../Scene/SceneMng.h"
#include "../../../common/SoundPross.h"
#include "NodeFunc.h"

NodeState MoveFunc::operator()(Obj& obj, float delta)
{
	// �ړ�������
	obj.MovePos(delta);

	// �A�C�h����ԃt���O��false�ɂ���(Unit�p����)
	auto unit = dynamic_cast<Unit*>(&obj);
	if (unit == nullptr)
	{
		return NodeState::Success;
	}
	unit->SetIdleFlag(false);
	return NodeState::Success;
}

NodeState RotaFunc::operator()(Obj& obj, float delta)
{
	// �G����]������
	dynamic_cast<Enemy&>(obj).Rotation(delta);
	return NodeState::Success;
}


NodeState NaviSetRoutePatrolFunc::operator()(Obj& obj, float delta)
{
	// �i�r���擾
	auto& navi = dynamic_cast<Navi&>(obj.GetComponent(ComponentID::Navi));

	// ����n�_���S�[���Ɍo�H��T������
	navi.Search( obj.GetPos(), dynamic_cast<PatrolBaseNavi&>(obj.GetComponent(ComponentID::PatrolNavi)).GetPos());
	return NodeState::Success;
}

NodeState NaviMoveFunc::operator()(Obj& obj, float delta)
{
	// �i�r���擾
	auto& navi = dynamic_cast<Navi&>(obj.GetComponent(ComponentID::Navi));

	// �x�N�g���Ɛ��K�������x�N�g����p��
	const auto&& moveVec = navi.GetNaviPos() - obj.GetPos();
	auto moveVecN = moveVec.Normalized();
	
	// �������r
	if (moveVec.SqMagnitude() < Math::Square(obj.GetSpeed() * delta *  obj.GetSpMag()))
	{
		// �ʏ�̈ړ��x�N�g���̕��������Ƃ����̂܂܃i�r�̎w���̍��W��
		obj.SetPos(navi.GetNaviPos());
		return NodeState::Success;
	}
	
	// �ړ��x�N�g�����Z�b�g
	obj.SetMoveVec(moveVecN);

	// �ړ�����
	obj.SetPos(obj.GetPos() + (moveVecN * (obj.GetSpeed() * delta * obj.GetSpMag())));

	// �A�C�h����ԃt���O��false�ɂ���
	auto unit = dynamic_cast<Unit*>(&obj);
	if (unit == nullptr)
	{
		return NodeState::Success;
	}
	unit->SetIdleFlag(false);
	return NodeState::Success;
}

NodeState PatrolMoveFunc::operator()(Obj& obj, float delta)
{
	// ����p�̃i�r���擾
	auto& navi = dynamic_cast<PatrolBaseNavi&>(obj.GetComponent(ComponentID::PatrolNavi));

	// �x�N�g���Ɛ��K���������̂�p��
	const auto&& moveVec = navi.GetMovePos() - obj.GetPos();
	auto moveVecN = moveVec.Normalized();

	// �������r
	if (moveVec.SqMagnitude() < Math::Square(obj.GetSpeed() * delta * obj.GetSpMag()))
	{
		// �ʏ�̈ړ��x�N�g���̕��������Ƃ����̂܂܃i�r�̎w���̍��W��
		obj.SetPos(navi.GetMovePos());
		return NodeState::Success;
	}

	// �ړ��x�N�g�����Z�b�g
	obj.SetMoveVec(moveVecN);

	// �ړ�����
	obj.SetPos(obj.GetPos() + (moveVecN * (obj.GetSpeed() * delta * obj.GetSpMag())));

	// �A�C�h����ԃt���O��false�ɂ���
	auto unit = dynamic_cast<Unit*>(&obj);
	if (unit == nullptr)
	{
		return NodeState::Faild;
	}
	unit->SetIdleFlag(false);
	return NodeState::Success;
}

NodeState NaviWaitSearchFunc::operator()(Obj& obj, float delta)
{
	// �i�r���擾
	auto& navi = dynamic_cast<Navi&>(obj.GetComponent(ComponentID::Navi));

	// ��Ԃ��`�F�b�N
	if (navi.GetNaviState() != NaviState::Search)
	{
		// search�ȊO�̎�����
		return NodeState::Success;
	}
	// search���Ȃ̂ŏ�����
	return NodeState::Now;
}


NodeState PatrolNaviStartFunc::operator()(Obj& obj, float delta)
{
	// ����p�i�r���擾
	auto patrol = dynamic_cast<PatrolBaseNavi*>(&obj.GetComponent(ComponentID::PatrolNavi));
	if (patrol == nullptr)
	{
		return NodeState::Faild;
	}

	// ������J�n����
	patrol->Start();
	return NodeState::Success;
}

NodeState PatrolNaviStopFunc::operator()(Obj& obj, float delta)
{
	// ����p�i�r���擾
	auto patrol = dynamic_cast<PatrolBaseNavi*>(&obj.GetComponent(ComponentID::PatrolNavi));
	if (patrol == nullptr)
	{
		return NodeState::Faild;
	}
	// ������~����
	patrol->Stop();
	return NodeState::Success;
}

NodeState AnimCorrectMoveVecToAnim::operator()(Obj& obj, float delta)
{
	auto unit = dynamic_cast<Unit*>(&obj);
	if (unit == nullptr)
	{
		return NodeState::Faild;
	}
	// �A�j���[�V������␳����
	unit->CorrectMoveVecToAnim();
	return NodeState::Success;
}



NodeState EnemyRandRotaFunc::operator()(Obj& obj, float delta)
{
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return NodeState::Faild;
	}
	enemy->SetRotationAngle();
	return NodeState::Success;
}



EnemyWaitStateTimeFunc::EnemyWaitStateTimeFunc(float inWaitTime) :
	waitTime_{inWaitTime}
{
}

NodeState EnemyWaitStateTimeFunc::operator()(Obj& obj, float delta)
{
	// �G���擾
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return NodeState::Faild;
	}

	// ���Ԃ��`�F�b�N����
	if (enemy->GetStateTime() > waitTime_)
	{
		// �ҋ@���Ԃ��z���Ă����琬��
		return NodeState::Success;
	}
	// �A�C�h����Ԃɂ���
	enemy->SetIdleFlag(true);

	// ���Ԃ��z���ĂȂ��̂ŏ�����
	return NodeState::Now;
}

EnemySetStateFunc::EnemySetStateFunc(EnemyState state) :
	state_{state}
{
}

NodeState EnemySetStateFunc::operator()(Obj& obj, float delta)
{
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return NodeState::Faild;
	}
	// �G�̏�Ԃ��Z�b�g����
	enemy->SetState(state_);
	return NodeState::Success;
}

EnemyReactionAnimFunc::EnemyReactionAnimFunc(const std::string& name) :
	animName_{name}
{
}

NodeState EnemyReactionAnimFunc::operator()(Obj& obj, float delta)
{
	auto anim = dynamic_cast<Animation*>(&obj.GetComponent(ComponentID::Animator, 1));
	if (anim == nullptr)
	{
		return NodeState::Faild;
	}
	anim->SetState(animName_);
	return NodeState::Success;
}

RandLRPosFunc::RandLRPosFunc(float val) :
	valMax_{val}
{
}

NodeState RandLRPosFunc::operator()(Obj& obj, float delta)
{
	// �����Ń����_���ɍ��E�ɗh�炷
	auto pos = obj.GetPos();
	pos.x += Rand.GetRandom(-valMax_, valMax_);
	obj.SetPos(pos);
	return NodeState::Success;
}

NodeState DeleteObjFunc::operator()(Obj& obj, float delta)
{
	// �I�u�W�F�N�g�����̕��������g���폜������
	obj.Obj::Destroy();
	return NodeState::Success;
}

NodeState EnemyTraceFunc::operator()(Obj& obj, float delta)
{
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return NodeState::Faild;
	}
	// �G�̏�Ԃ��Z�b�g����
	enemy->TraceStart();
	return NodeState::Success;
}

NodeState EnemyStartNaviTargetFunc::operator()(Obj& obj, float delta)
{
	auto enemy = dynamic_cast<Enemy*>(&obj);
	if (enemy == nullptr)
	{
		return NodeState::Faild;
	}
	// �G�̏�Ԃ��Z�b�g����
	
	return enemy->StartMoveNavi() ? NodeState::Success : NodeState::Faild;
}

PlaySEFunc::PlaySEFunc(int seID) :
	se_{ static_cast<SOUNDNAME_SE>(seID)}
{
}

NodeState PlaySEFunc::operator()(Obj& obj, float delta)
{
	SoundProcess::PlayBackSound(static_cast<SoundProcess::SOUNDNAME_SE>(se_), SoundProcess::GetSEVolumeEntire (), false);
	return NodeState::Success;
}
