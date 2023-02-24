#pragma once
#include <functional>
#include <vector>
#include "../../Math.h"
#include "CameraModeBase.h"

class Obj;
class Controller;

class StartEventMode :
    public CameraModeBase
{
public:
	StartEventMode(Controller& controller,Camera& camera, const std::weak_ptr<Obj>& start, const std::weak_ptr<Obj>& end);
	~StartEventMode();
private:
	void Update(Camera& camera, float delta) final;
	void Draw(Camera& camera) final;

	/// <summary> �}�b�v�������邽�߂̈ړ� </summary>
	/// <param name="camera"></param>
	void MoveMapUpdate(Camera& camera,float dleta);

	/// <summary> �^�[�Q�b�g�܂ňړ� </summary>
	/// <param name="camera"></param>
	void MoveTargetUpdate(Camera& camera, float dleta);
	
	/// <summary> �^�[�Q�b�g��\�����đҋ@���� </summary>
	/// <param name="camera"></param>
	void WaitTargetUpdate(Camera& camera, float dleta);

	/// <summary> ���̈ʒu�ɖ߂� </summary>
	/// <param name="camera"></param>
	void BackUpdate(Camera& camera, float dleta);

	void Skip(Camera& camera);

	void Loaded(void) final;

	// �J�n�ʒu
	std::weak_ptr<Obj> start_;

	// �Г��̏I�_
	std::weak_ptr<Obj> end_;

	// �J�n���Ɍ�������W
	std::vector<Math::Vector2> positions_;

	// ��L�̃C�e���[�^�[
	std::vector<Math::Vector2>::const_iterator posItr_;

	// �ړ��J�n���W
	Math::Vector2 startPos_;

	// �ړ��I�����W
	Math::Vector2 endPos_;

	// �X�L�b�v���̔{������l
	float skipVal_;

	// 
	std::function<void(Camera&,float)> updateFunc_;
	
	// �㉺�̘g�̒l
	float frameVal_;

	// ���͏��
	Controller& controller_;

	// �X�L�b�v�ł��邱�Ƃ�\��������W
	Math::Vector2 skipPos_;

	// ���͏��̈ꕶ�����̑傫��
	int inputOffset_;

	// ���͏��̉摜�̃L�[
	std::string skipInput_;

	int inputCode_;

	float skipTime_;

	enum class Mode
	{
		MoveMap,
		Target,
		Wait,
		Back
	};

};

