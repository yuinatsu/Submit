#pragma once
#include "../Controller.h"

// �L�[�{�[�h�p
class Keyboard :
    public Controller
{
public:
	Keyboard();
	~Keyboard();

	// ������
	bool Init(void)override;
	
	// �X�V Update(deltaTime)
	void Update(float delta) override;

	// �R���g���[���[�^�C�v�̎擾private:
	CntType GetCntType(void) override { return CntType::Key; }

	// �L�[�f�[�^�̎擾
	const std::array<char, 256>& GetKeyData(void) const&
	{
		return keyData_;
	}
private:

	// �J�[�\���̍��W���Z�b�g 
	void SetCursorPos(const Vector2& pos = lpSceneMng.screenSize_<float> / 2.0f) final;

	// �L�[�f�[�^�i�[�p
	std::array<char, 256> keyData_ = {};
};

