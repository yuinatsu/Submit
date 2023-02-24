#pragma once
#include "SCN_ID.h"
#include "Vector2.h"
#include "common.h"

class RuleScene
{
public:
	RuleScene();												// RuleScene�̐���
	~RuleScene();												// RuleScene�̔j��
	SCN_ID Update(char* keyData, char* keyDataOld);				// RuleScene�̍X�V
	void Draw(void);											// RuleScene�̕`��
	bool Init(void);											// RuleScene�̏�����
	bool Relese(void);											// RuleScene�̉��
private:
	Vector2 spacePos_;											// "PUSH SPACE KEY"�̍��W
	Vector2 screenSize_;										// �X�N���[���T�C�Y
	int SpaceImage_;											// "PUSH SPACE KEY"
	int blinkCnt_ = 0;											// �_�ŃJ�E���^
};

