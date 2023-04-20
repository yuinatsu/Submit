#pragma once
#include "Controller.h"
#include "../Common/Vector2.h"

class Keyboard :
	public Controller
{
public:
	Keyboard();
	~Keyboard() {}
	// �X�V
	void Update(double delta) override;
	// �R���g���[���[�^�C�v�̎擾
	CntType GetCntType(void) override { return CntType::Key; }
private:
	// �L�[�f�[�^�i�[�p
	std::array<char, 256> keyData_ = {};
};

