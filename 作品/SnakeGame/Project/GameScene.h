#pragma once
#include "SCN_ID.h"
#include "MapCtl.h"
#include "Player.h"
#include "common.h"

#define CALL_IMAGE_SIZE_X 300							// ���}���S���T�C�Y
#define CALL_IMAGE_SIZE_Y (240 / CALL_MAX)				// ���}���S�c�T�C�Y
#define CALL_MAX 3										// ���}���S����
#define START_CALL_MAX 2								// �J�n���}���S����
#define CALL_COUNT_MAX 100								// ���}�Ԋu

class GameScene
{
public:
	GameScene();												// GameScene�̐���
	~GameScene();												// GameScene�̔j��
	SCN_ID Update(char* keyData, char* keyDataOld);				// GameScene�̍X�V
	void Draw(void);											// GameScene�̕`��
	void StartCallDraw(void);									// �Q�[���J�n�O�̃J�E���g�_�E���\��
	bool Init(CHAR_ID char1, CHAR_ID char2);					// GaneScene�̏�����							// Init(1P�̃L����ID, 2P�̃L����ID)
	bool Relese(void);											// GameScene�̉��

	Player player[PLAYER_MAX];								// �v���C���[					// player[�v���C���[����]
private:
	MapCtl mapCtl;											// �u���b�N�̏��Ǘ�
	int bgImage_;											// �Q�[���w�i
	int callImage[CALL_MAX];								// �R�[�����S					// callImage[���}���S����]
	bool deadFlag_ = false;									// ���S�t���O
	bool callFlag_ = true;
	int callCnt_;											// ���}�J�E���^
	Vector2 screenSize_;									// �X�N���[���T�C�Y
};

