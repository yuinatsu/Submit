#pragma once
#include "ButtonBase.h"
#include <functional>

// ���ʂȂǂ̒����Ɏg����UI
class Slider :
    public ButtonBase
{
	using ValueGetFunc = std::function<void(const float)>;
public:
	Slider(ValueGetFunc&& valueGetFunc, const std::filesystem::path& sliderBarPath, const std::filesystem::path& sliderTabPath, float defaultValue,const Vector2& pos);
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

	// �O������l��m�邽�߂Ɏg��
	ValueGetFunc valueGetFunc_;

	// �X���C�_�[�̃o�[�̉摜
	SharedGraphicHandle sliderBar_;

	// �X���C�_�[�̎����Ẳ摜
	SharedGraphicHandle sliderTab_;

	// �X���C�_�[�̎�����̃T�C�Y
	Vector2 sliderTabSize_;

	// �X���C�_�[�̎�����̍��W
	Vector2 sliderTabPos_;

	// �l(0�`1�͈̔�)
	float value_;
};

