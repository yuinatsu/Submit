#pragma once
#include <string>
#include "../../../Object/Unit/EnemyState.h"
#include "../NodeState.h"

class Obj;

enum class SOUNDNAME_SE;

// �ړ�
struct MoveFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// ��]
struct RotaFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// �G����]������
struct EnemyRandRotaFunc
{
	NodeState operator()(Obj& obj, float delta);
};



// Navi�̖ړI�n������n�_�ɂ���
struct NaviSetRoutePatrolFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// Navi�ɏ]��Obj���ړ�������
struct NaviMoveFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// PatrolNavi�ɏ]��Obj���ړ�������
struct PatrolMoveFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// Navi�̒T����ҋ@����
struct NaviWaitSearchFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// ������J�n����
struct PatrolNaviStartFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// ������~����
struct PatrolNaviStopFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// MoveVec�����ƂɃA�j���[�V�������Z�b�g����
struct AnimCorrectMoveVecToAnim
{
	NodeState operator()(Obj& obj, float delta);
};


// �G�̏�Ԏ��Ԃ��w�莞�ԂɂȂ�܂ő҂���
struct EnemyWaitStateTimeFunc
{
	EnemyWaitStateTimeFunc(float inMaxTime);
	NodeState operator()(Obj& obj, float delta);
private:
	const float waitTime_;
};

// �G�̏�Ԃ��Z�b�g���鏈��
struct EnemySetStateFunc
{
	EnemySetStateFunc(EnemyState state);
	NodeState operator()(Obj& obj, float delta);
private:
	const EnemyState state_;

};

// �G�̔����p�A�j���[�V�������Z�b�g���鏈��
struct EnemyReactionAnimFunc
{
	EnemyReactionAnimFunc(const std::string& name);
	NodeState operator()(Obj& obj, float delta);
private:
	const std::string animName_;
};

// �����_���ɍ��E�ɗh��鏈��
struct RandLRPosFunc
{
	RandLRPosFunc(float val);
	NodeState operator()(Obj& obj, float delta);
private:
	const float valMax_;
};

// �I�u�W�F�N�g��j�����鏈��
struct DeleteObjFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// �G���ǐՎ��̏���
struct EnemyTraceFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// �G�����^�[�Q�b�g���W��Navi���J�n����
struct EnemyStartNaviTargetFunc
{
	NodeState operator()(Obj& obj, float delta);
};

// SE�̍Đ����鏈��
struct PlaySEFunc
{
	PlaySEFunc(int seID);
	NodeState operator()(Obj& obj, float delta);
	SOUNDNAME_SE se_;
};

