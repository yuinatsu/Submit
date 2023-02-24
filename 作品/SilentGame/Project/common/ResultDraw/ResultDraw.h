#pragma once
#include <string>
#include <functional>
#include "../Math.h"

// ���U���g�V�[���Ō��ʂ�\������N���X�̊��
class ResultDraw
{
public:

	/// <summary> �R���X�g���N�^ </summary>
	/// <param name="startTime"> �J�n����(�Z�Z�b��ɂ݂�����) </param>
	/// <param name="pos"> ���S���W </param>
	ResultDraw(float startTime, const Math::Vector2& pos);

	virtual ~ResultDraw();

	/// <summary> �A�b�v�f�[�g </summary>
	/// <param name="nowTime"> �o�ߎ��� </param>
	void Update(const float& nowTime);

	/// <summary> �`�� </summary>
	/// <param name=""></param>
	void Draw(void);

	/// <summary> �`��J�n�������ǂ��� </summary>
	/// <param name=""></param>
	/// <returns> �`��J�n���Ă�����true </returns>
	const bool IsStartedDraw(void) const { return drawTime_ - fadeTime_ > 0.0f; }
protected:

	/// <summary> �������Ȃ��`��p�̃A�b�v�f�[�g </summary>
	/// <param name="nowTime"></param>
	void UpdateNon(const float& nowTime);

	/// <summary> �`�掞�p�̃A�b�v�f�[�g </summary>
	/// <param name="nowTime"></param>
	void UpdateDraw(const float& nowTime);

	/// <summary> �`�悷�� </summary>
	/// <param name=""></param>
	virtual void DrawOwn(void) = 0;

	// �`�揈��
	std::function<void(void)> drawFunc_;

	// �A�b�v�f�[�g
	std::function<void(const float&)> updateFunc_;

	// �`��J�n�܂ł̎���
	const float drawStartTime_;

	// �`��J�n�ォ��̌o�ߎ���
	float drawTime_;

	// ���S���W
	Math::Vector2 pos_;

	static constexpr float fadeTime_{ 0.5f };
};

