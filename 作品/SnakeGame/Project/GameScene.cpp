#include <DxLib.h>
#include "GameScene.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

// GameScene�̐���
//------------------
GameScene::GameScene()
{
	TRACE("GameScene�̐���\n");
}

// GameScene�̔j��
//------------------
GameScene::~GameScene()
{
	TRACE("GameScene�̔j��\n");
}

// GameScene�̍X�V
//------------------
SCN_ID GameScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("GameScene_Update\n");

	for (int cnt = 0; cnt < START_CALL_MAX; cnt++)						// call.png�̏�Q����
	{
		if (callCnt_ < CALL_COUNT_MAX + (CALL_COUNT_MAX * cnt))			// �J�E���g����萔���Z
		{
			callCnt_++;
			return SCN_ID::GAME;										// ��萔�̃J�E���g�𒴂���܂ł�ID��GAME�ŉ�
		}
	}
	if (callCnt_ >= CALL_COUNT_MAX * 2)									// ��莞�Ԍo�߂ŃJ�E���g�_�E���I��
	{
		callFlag_ = false;
	}

	mapCtl.Update();													// mapCtl.Update()�̌Ăяo��

	int deadCnt = 0;																// state_��DEAD�̃L�����̃J�E���^
	for (int no = 0; no < PLAYER_MAX; no++)											// �v���C���[����(����͂Q�l)��Update���Ăяo��
	{
		if (player[no].Update(mapCtl, keyData, keyDataOld) == PL_ST::DEAD)			// �v���C���[��state_��DEAD��������deadCnt���J�E���g�A�b�v
		{
			deadCnt++;
		}
	}
	
	if (keyData[KEY_INPUT_F12] && !keyDataOld[KEY_INPUT_F12])				// �����^�C�g���ړ�
	{
		return SCN_ID::TITLE;
	}
	if (deadCnt > 0)								// �v���C���[���P�l�ł�����ł����烊�U���g�V�[���Ɉړ�
	{
		deadFlag_ = true;							// ���Z�b�g
		callFlag_ = true;							// ���Z�b�g
		return SCN_ID::RESULT;
	}
	return SCN_ID::GAME;							// �v���C���[���ǂ���������Ă�����Q�[���V�[���𑱍s


}

// GameScene�̕`��
//------------------
void GameScene::Draw(void)
{
	TRACE("GameScene_Draw\n");

	DrawGraph(0, 0, bgImage_, true);			// �Q�[���w�i

	mapCtl.Draw();								// mapCtl.Draw()�̌Ăяo��

	for (int no = 0; no < PLAYER_MAX; no++)		// �v���C���[���Ƃ̃L�����`�b�v�ƃL�����A�C�R��
	{
		player[no].Draw();
	}

	if (callFlag_ == true)
	{
		StartCallDraw();							// �Q�[���J�n�O�̃J�E���g�_�E���\��
	}

	if (deadFlag_ == true)						// �Q�[���I���\��
	{
		DrawGraph((screenSize_.x - CALL_IMAGE_SIZE_X) / 2, (screenSize_.y - CALL_IMAGE_SIZE_Y) / 2, callImage[2], true);
	}
}

// �Q�[���J�n�O�̃J�E���g�_�E���\��
//-----------------------------------
void GameScene::StartCallDraw(void)
{
	for (int cnt = 0; cnt < START_CALL_MAX; cnt++)								// call.png�̏�Q�����̕\��
	{
		if (!(callCnt_ >= CALL_COUNT_MAX + (cnt * CALL_COUNT_MAX)))				// �J�E���g����萔�s������\�����Ȃ��悤�ɂ���
		{
			if (callCnt_ >= 50 * (cnt + (cnt * 2)))								// callImage����萔�\��
			{
				DrawGraph((screenSize_.x - CALL_IMAGE_SIZE_X) / 2, (screenSize_.y - CALL_IMAGE_SIZE_Y) / 2, callImage[cnt], true);
			}
		}
	}
}

// GaneScene�̏�����
//--------------------
bool GameScene::Init(CHAR_ID char1, CHAR_ID char2)
{
	TRACE("GameScene_Init\n");

	bool rtnFlag = true;
	if ((bgImage_ = LoadGraph("Resource/image/bg.png")) == -1)				// �Q�[���w�i
	{
		TRACE("�ǂݍ��݃G���[1\n");
		rtnFlag = false;
	}
	if ((LoadDivGraph("Resource/image/call.png", CALL_MAX, 1, CALL_MAX, CALL_IMAGE_SIZE_X, CALL_IMAGE_SIZE_Y, callImage)) == -1)				// ���}���S
	{
		TRACE("�ǂݍ��݃G���[2\n");
		rtnFlag = false;
	}

	GetScreenState(&screenSize_.x, &screenSize_.y, nullptr);				// �X�N���[���T�C�Y�擾

	mapCtl.Init();															// mapCtl��Init���Ăяo��		// ���̂��̂���Q�Ƃ���₷�����̂���Init���Ă���

	player[0].Init(char1);													// player[0]��char1�̏�������
	player[1].Init(char2);													// player[1]��char2�̏�������

	callCnt_ = 0;															// �J�E���g�_�E���p�J�E���^
	deadFlag_ = false;														// ���S�t���O

	return rtnFlag;
}

// GameScene�̉��
//------------------
bool GameScene::Relese(void)
{
	TRACE("GameScene_Relese\n");

	for (int no = 0; no < PLAYER_MAX; no++)						// �v���C���[���Ƃ̃L�������
	{
		player[no].Relese();
	}

	mapCtl.Relese();											// mapCtl�̉��				// Relese���鏇�Ԃ�Init�������ԂƋt

	for (int no = 0; no < CALL_MAX; no++)						// ���}���S�̉��
	{
		DeleteGraph(callImage[no]);
	}

	DeleteGraph(bgImage_);										// �Q�[���w�i�̉��

	return true;
}
