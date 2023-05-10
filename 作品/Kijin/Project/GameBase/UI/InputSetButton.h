#pragma once
#include "ButtonBase.h"
#include "../Common/Input/InputID.h"

// �ݒ��ʂ̃L�[��PAD�̐ݒ�p�{�^��
class InputSetButton :
	public ButtonBase
{
public:
	InputSetButton(InputID id, const Vector2& pos);
private:
	void Update(float delta, BaseScene& scene, ObjectManager& objMng, Controller& controller) final;
	void Draw(int mainScr) final;
	void Push(BaseScene& scene, Controller& controller) final;
	void HitCursor(void) final;
	void NotHitCursor(void)final;

	UiID GetUIID(void) final
	{
		return UiID::Button;
	}
	void Loaded(BaseScene& scene) final;

	/// <summary>
	/// �J�[�\�����������Ă鎞�̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawHitCursor(void);

	/// <summary>
	/// �J�[�\�����������ĂȂ��Ƃ��̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawNonHitCursor(void);

	/// <summary>
	/// xbox�n�̃R���g���[���[�g�p���`��
	/// </summary>
	/// <param name=""></param>
	void DrawNowXboxCode(void);

	/// <summary>
	/// �L�[�{�[�h�g�p���̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawNowKeyCode(void);

	/// <summary>
	/// ps�n�̃R���g���[���[�g�p���`��
	/// </summary>
	/// <param name=""></param>
	void DrawNowPsCode(void);

	// �`�揈��
	void (InputSetButton::* draw_)(void);

	// ���݂̐ݒ肳�ꂽ�R�[�h�̕`��
	void (InputSetButton::* drawNowCode_)(void);

	// InputID
	InputID id_;

	// inputID�̕\���p�O���t�B�b�N
	SharedDivGraphicHandle inputIDInfo_;

	// ���݂̓��͏��p�O���t�B�b�N
	SharedDivGraphicHandle inputInfo_;

	// �{�^���̃O���t�B�b�N
	SharedGraphicHandle button_;

	// �N���b�N���̘g
	SharedGraphicHandle frame_;

	// ���݂̃R�[�h
	int nowCode_;
};

