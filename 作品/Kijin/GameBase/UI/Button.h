#pragma once
#include "ButtonBase.h"
#include <functional>

class BaseScene;

class Button :
    public ButtonBase
{
	using PushFunc = std::function<void(BaseScene&)>;
public:
	Button(PushFunc&& pushFunc, const std::filesystem::path& btnPath, const std::filesystem::path& hitCursorPath, const Vector2& pos);
	void Update(float delta, ObjectManager& objMng, Controller& controller);
	void Draw();

private:
	void Push(BaseScene& scene, Controller& controller) final;
	void HitCursor(void) final;
	void NotHitCursor(void)final;

	UiID GetUIID(void) final
	{
		return UiID::Button;
	}
	void Loaded(void) final;

	/// <summary>
	/// �J�[�\���Ɠ������Ă��鎞�̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawHitCursor(void);

	/// <summary>
	/// �J�[�\���Ɠ������ĂȂ��Ƃ��̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawNotHitCursor(void);

	// �`��
	void(Button::* draw_)(void);

	// �{�^��
	SharedGraphicHandle btnHandle_;

	// �J�[�\���Ɠ������Ă鎞
	SharedGraphicHandle hitCursorHandle_;

	// �{�^�����������Ƃ��̏���
	PushFunc pushFunc_;
};

