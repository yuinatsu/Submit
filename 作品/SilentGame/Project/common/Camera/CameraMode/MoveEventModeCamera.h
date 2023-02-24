#pragma once
#include <memory>
#include "CameraModeBase.h"

class Obj;

// �����ړ�����J����
class MoveEventModeCamera :
	public CameraModeBase
{
public:
	MoveEventModeCamera(float allTime,float waitTime,const std::weak_ptr<Obj>& start,const std::weak_ptr<Obj>& end);
	~MoveEventModeCamera();
	void Update(Camera& camera, float delta) final;
	void Draw(Camera& camera) final;
private:

	// �J�n�ʒu
	std::weak_ptr<Obj> start_;

	// �Г��̏I�_
	std::weak_ptr<Obj> end_;

	// �J�����̃��[�h�̎���
	float allTime_;

	// �ҋ@����
	float waitTime_;

	// �^�[�Q�b�g�̕��։�������J�����̏��
	enum class EventCamState
	{
		Move,		// �ړ���
		Wait,
		Back		// �߂�
	};

	// �^�[�Q�b�g�̕��։�������J�����̌��݂̏��
	EventCamState eventCamState_{ EventCamState::Move };
};

