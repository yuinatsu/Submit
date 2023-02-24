#pragma once
#include <DxLib.h>
#include "Controller.h"
class PadInput :
	public Controller
{
public:
	PadInput();
	~PadInput();
	bool Init(void) override;										// ������
	void Update(double delta) override;								// �X�V Update(deltaTime)
	void StickUpdate(void);											// �X�e�B�b�N�̍X�V
	CntType GetCntType(void) override { return CntType::Pad; };		// �R���g���[���[�^�C�v�̎擾
private:
	XINPUT_STATE input_;											// �R���g���[���[�̏��
	static int count_;												// �����݂�pad�̐�
	int padID_;														// ������padID
};

