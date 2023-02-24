#pragma once

class Obj;

// ���W���`�F�b�N����
struct CheckPosFunc
{
	bool operator()(Obj& obj, float delta);
};

// �G�̊p�x���`�F�b�N����
struct EnemyCheckAngleFunc
{
	bool operator()(Obj& obj, float delta);
};

enum class EnemyState;
// �G�̏�Ԃ��`�F�b�N����
struct EnemyCheckStateFunc
{
	EnemyCheckStateFunc(EnemyState state);
	bool operator()(Obj& obj, float delta);
private:
	const EnemyState ckState_;
};

// �G�̏�Ԃ���莞�Ԍo���������`�F�b�N����
struct EnemyIsOverStateTimeFunc
{
	EnemyIsOverStateTimeFunc(float limitTime);
	bool operator()(Obj& obj, float delta);
private:
	const float limitTime_;
};

// ���E�̔��肪�������Ă邩
struct FanColIsHitFunc
{
	bool operator()(Obj& obj, float delta);
};

// �����Ă���Navi���ړI�n�����������`�F�b�N����
struct NaviStateCheckFindFunc
{
	bool operator()(Obj& obj, float delta);
};

// Navi��search���ł������Ă��Ȃ��Ƃ�
struct NaviCheckDestFunc
{
	bool operator()(Obj& obj, float delta);
};

// ���񂪒�~���łȂ��Ƃ�
struct PatrolCheckNotIsStop
{
	bool operator()(Obj& obj, float delta);
};

// �ړ��ł��邩?
struct IsMoveFunc
{
	bool operator()(Obj& obj, float delta);
};


