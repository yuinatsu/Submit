#pragma once
#include "SCN_ID.h"
#include "Player.h"
#include "CharImage.h"
#include "common.h"
#include "Vector2.h"

#define FINISH_COUNT_MAX 100					// �I��荇�}�̃J�E���g

class ResultScene
{
public:
	ResultScene();														// ResultScene�̐���
	~ResultScene();														// ResultScene�̔j��
	SCN_ID Update(char* keyData, char* keyDataOld);						// ResultScene�̍X�V
	void Draw(void);													// ResultScene�̕`��
	bool Init(CHAR_ID winnerID);										// ResultScene�̏�����				// Init(���������L������ID)
	bool Relese(void);													// ResultScene�̉��
private:
	Player player[PLAYER_MAX];									// �v���C���[[�v���C���[���l��]
	CharImage charImage_;										// �L�����C���[�W
	CHAR_ID winnerID_;											// ���������L������ID
	Vector2 screenSize_;										// �X�N���[���T�C�Y
	Vector2 picPos_;											// �L���������G�̍��W
	Vector2 spacePos_;											// "PUSH SPACE KEY"�̍��W
	int bgImage_;												// �Q�[���w�i
	int winMesImage[static_cast<int>(CHAR_ID::MAX)];			// �����҃��b�Z�[�W���S[���������L�����̎��]
	int SpaceImage_;											// "PUSH SPACE KEY"
	int finishCnt_;												// ���}�J�E���^
	int blinkCnt_ = 0;											// �_�ŃJ�E���^
};

