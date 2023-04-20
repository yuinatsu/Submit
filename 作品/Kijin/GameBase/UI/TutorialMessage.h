#pragma once
#include "UiBase.h"
#include <vector>
#include <unordered_map>

enum class TutorialType;

class TutorialMessage :
    public UiBase
{
public:
	TutorialMessage();
private:

	enum class DrawType
	{
		Mess,
		Controller
	};

	void Update(float delta, BaseScene& scene, ObjectManager& objMng, Controller& controller) final;
	void Draw(int mainScr) final;
	UiID GetUIID(void) final
	{
		return UiID::TutorialMess;
	}

	/// <summary>
	/// �`���[�g���A�����b�Z�[�W��`��
	/// </summary>
	/// <param name=""></param>
	void DrawMess(void);

	/// <summary>
	/// �`���[�g���A���̑��상�b�Z�[�W��`��
	/// </summary>
	/// <param name=""></param>
	void DrawControllerMess(void);

	// ���݂̃`���[�g���A���̎��
	TutorialType nowType_;

	// ���b�Z�[�W�̉摜
	std::vector<SharedGraphicHandle> messages_;

	// �L�[�Ƃ��p�b�h�̉摜
	SharedDivGraphicHandle controllerImg_;

	// �`��^�C�v�e�[�u��
	std::unordered_map<TutorialType, DrawType> drawTypeTbl_;

	// �`��֐�
	std::vector<void(TutorialMessage::*)(void)> drawFunc_;

	std::unordered_map<int, int>* codeTbl_;
};

