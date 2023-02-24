#pragma once
#include "Controller.h"

class Keyboard :
    public Controller
{
public:
	Keyboard();
	~Keyboard();
	bool Init(void)override;										// ������
	void Update(double delta) override;								// �X�V Update(deltaTime)
	CntType GetCntType(void) override { return CntType::Key; }		// �R���g���[���[�^�C�v�̎擾private:
private:
	std::array<char, 256> keyData_ = {};							// �L�[�f�[�^�i�[�p
};

