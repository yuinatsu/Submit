#pragma once
#include <DxLib.h>
#include "../Controller.h"

constexpr int motionRange = 30000;                                  // �X�e�B�b�N�̔�������

// �Q�[���p�b�h�p
class Pad :
	public Controller
{
public:
	Pad(int padType);
	~Pad();

	// ������ 
	bool Init(void) override;

	// �A�b�v�f�[�g 
	void Update(float delta) override;

	// �R���g���[���[�̃^�C�v�̎擾 
	CntType GetCntType(void) override { return CntType::Pad; };

	// �Q�[���p�b�h�̏����擾 
	const DINPUT_JOYSTATE& GetPadState(void) const&
	{
		return state_;
	}

	const int GetPadType(void) const;
private:

	// �J�[�\���̍��W���Z�b�g 
	void SetCursorPos(const Vector2& pos = lpSceneMng.screenSize_<float> / 2.0f) final;

	// �v���C�X�e�[�V�����n�̃p�b�h�̉E�X�e�B�b�N�̍X�V 
	void UpdatePsPad(float delta);

	// xbox�n�̃p�b�h�̉E�X�e�B�b�N�̍X�V 
	void UpdateXboxPad(float delta);

	// �Q�[���p�b�h�̏��
	DINPUT_JOYSTATE state_;

	// ���݂̃Q�[���p�b�h�̎��
	int nowPadType_;

	// �X�V����
	void(Pad::* update_)(float);
};

