#pragma once
#include <DxLib.h>
#include "../Controller.h"

constexpr int motionRange = 30000;                                  // �X�e�B�b�N�̔�������

class Pad :
	public Controller
{
public:
	Pad(int padType);
	~Pad();
	bool Init(void) override;										// ������
	void Update(float delta) override;								// �X�V Update(deltaTime)
	CntType GetCntType(void) override { return CntType::Pad; };		// �R���g���[���[�^�C�v�̎擾
private:
	void SetCursorPos(const Vector2& pos = lpApp.screenSize_<float> / 2.0f) final;

	/// <summary>
	/// �v���C�X�e�[�V�����n�̃p�b�h�̉E�X�e�B�b�N�̍X�V
	/// </summary>
	/// <param name=""></param>
	void UpdatePsPad(float delta);

	/// <summary>
	/// xbox�n�̃p�b�h�̉E�X�e�B�b�N�̍X�V
	/// </summary>
	/// <param name=""></param>
	void UpdateXboxPad(float delta);

	DINPUT_JOYSTATE state_;
	int padState_;
	int nowPadType_;
	void(Pad::* update_)(float);
};

