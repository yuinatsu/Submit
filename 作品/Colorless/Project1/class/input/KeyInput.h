#pragma once
#include "Controller.h"
class KeyInput :
	public Controller
{
public:
	KeyInput();
	~KeyInput();
	bool Init(void)override;										// ������
	void Update(double delta)override;								// �X�V Update(deltaTime)
	CntType GetCntType(void)override{ return CntType::Key; }		// �R���g���[���[�^�C�v�̎擾
private:
	std::array<char, 256> keyData_ = {};							// �L�[�f�[�^�i�[�p
};