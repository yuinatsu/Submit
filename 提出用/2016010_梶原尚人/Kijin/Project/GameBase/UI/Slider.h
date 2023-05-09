#pragma once
#include "ButtonBase.h"
#include <functional>

// ���ʂȂǂ̒����Ɏg����UI
class Slider :
    public ButtonBase
{
	using ValueGetFunc = std::function<void(const float)>;
public:
	Slider(
		const ValueGetFunc& valueGetFunc, 
		const std::filesystem::path& sliderBarPath, 
		const std::filesystem::path& sliderTabPath, 
		const std::filesystem::path& sliderValuePath, 
		float defaultValue,
		const Vector2& pos);
	void Update(float delta, BaseScene& scene, ObjectManager& objMng, Controller& controller);
	void Draw(int mainScr);

private:
	void Push(BaseScene& scene, Controller& controller) final;
	void HitCursor(void) final;
	void NotHitCursor(void)final;
	bool IsDecision(Controller& controller) const final;

	UiID GetUIID(void) final
	{
		return UiID::Button;
	}
	void Loaded(BaseScene& scene) final;

	// �O������l��m�邽�߂Ɏg��
	ValueGetFunc valueGetFunc_;

	// �X���C�_�[�̃o�[�̉摜
	SharedGraphicHandle sliderBar_;

	// �X���C�_�[�̎����Ẳ摜
	SharedGraphicHandle sliderTab_;

	// �X���C�_�[�̒l�͈̔͂̕\���p
	SharedGraphicHandle sliderValue_;

	// �X���C�_�[�̎�����̃T�C�Y
	Vector2 sliderTabSize_;

	// �X���C�_�[�̎�����̍��W
	Vector2 sliderTabPos_;

	// �X���C�_�[�̒l�̕\���摜�̃T�C�Y
	Vector2 sliderValueSize_;

	// �X���C�_�[�̒l�̕\���I�t�Z�b�g
	Vector2 sliderValueOffset_;

	// �l(0�`1�͈̔�)
	float value_;
};

