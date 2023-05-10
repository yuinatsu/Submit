#pragma once
#include "ButtonBase.h"
#include <functional>

class BaseScene;

class SwitchButton :
    public ButtonBase
{
public:
	using PushFunc = std::function<void(BaseScene&, bool)>;
	using UpdateFunc = std::function<bool(BaseScene&)>;
	SwitchButton(
		const PushFunc& pushFunc,
		const UpdateFunc& updateFunc,
		const std::filesystem::path& onPath,
		const std::filesystem::path& offPath,
		const std::filesystem::path& cursorPath,
		const Vector2& pos
	);
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
	/// �{�^�����I���̎��̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawOnButton(void);

	/// <summary>
	/// �{�^�����I�t�̎��̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawOffButton(void);

	/// <summary>
	/// �J�[�\������������̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawOnCursor(void);

	/// <summary>
	/// �J�[�\��������ĂȂ��Ƃ��̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawNotCursor(void);

	// �{�^���̕`��
	void(SwitchButton::* drawButton_)(void);

	// �t���[���̕`��(�J�[�\������������Ƃ���)
	void(SwitchButton::* drawFrame_)(void);

	// �I���t���O
    bool isOn_;

	// �{�^�����I���̎��̕`��
	SharedGraphicHandle onBtnHandle_;

	// �{�^�����I�t�̎��̕`��
	SharedGraphicHandle offBtnHandle_;

	// �J�[�\���Ɠ������Ă鎞
	SharedGraphicHandle hitCursorHandle_;

	// �{�^�����������Ƃ��̏���
	PushFunc pushFunc_;

	// �X�V����
	UpdateFunc updateFunc_;
};

