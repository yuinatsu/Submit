#include <DxLib.h>
#include "SelectScene.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

// SelestScene�̐���
//--------------------
SelectScene::SelectScene()
{
	TRACE("SelectScene�̐���\n");
}

// SelestScene�̔j��
//--------------------
SelectScene::~SelectScene()
{
	TRACE("SelectScene�̔j��\n");
}

// SelestScene�̍X�V
//--------------------
SCN_ID SelectScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("SelectScene_Update\n");

	for (int no = 0; no < PLAYER_MAX; no++)												// �v���C���[����
	{
		player[no].GetKeyState(keyData);												// �v���C���[�̃L�[�f�[�^�擾

		SelectChar(&player[no], DIR::LEFT, -1, CHAR_ID::REIMU, CHAR_ID::YOUMU);			// ���L�[��������REIMU,MARISA,CIRNO,SAKUYA,YOUMU�̏��ŃL�����Z���N�g		// �|�C���^�n��������ۂ� & �𓪂ɂ���
		SelectChar(&player[no], DIR::RIGHT, 1, CHAR_ID::YOUMU, CHAR_ID::REIMU);			// �E�L�[��������YOUMU,SAKUYA,CIRNO,MARISA,REIMU�̏��ŃL�����Z���N�g

		DesideChar(&player[no], DIR::UP, PL_ST::ALIVE);									// ��L�[�������ăL�����Z���N�g����
		DesideChar(&player[no], DIR::DOWN, PL_ST::NON);									// ���L�[�������ăL�����Z���N�g�L�����Z��
	}

	if (keyData[KEY_INPUT_F12] && !keyDataOld[KEY_INPUT_F12])							// �����^�C�g���ړ�
	{
		return SCN_ID::TITLE;
	}
	if ((player[0].state_ == PL_ST::ALIVE) && (player[1].state_ == PL_ST::ALIVE))		// �S���L���������肵����Q�[���V�[���Ɉړ�
	{
		return SCN_ID::GAME;
	}
	return SCN_ID::SELECT;
}

// �L�����Z���N�g�L�[���͊֐� 
//-----------------------------
bool SelectScene::SelectChar(Player* player, DIR dir, int count, CHAR_ID limID, CHAR_ID setID)		// Player�̌��� * ��t���邱�ƂŃ|�C���^�n�����s��
{
	if (player->state_ != PL_ST::NON)																// PL_ST��NON�ȊO�������烊�^�[������ (NON�ȊO�ŃL�[���͏������������Ȃ�����)
	{
		return false;
	}

	if (player->keyNow_[static_cast<int>(dir)] && !(player->keyOld_[static_cast<int>(dir)]))		// ���E�L�[�������ăL�����Z���N�g			-> ....�A���[����    �|�C���^���g���Ƃ��Ɏg��
	{
		if (player->charID_ != limID)
		{
			player->charID_ = static_cast<CHAR_ID>(static_cast<int>(player->charID_) + count);		// �ő�l�ɒB���Ă��Ȃ��ꍇ�L�����`��X�N���[��
		}
		else
		{
			player->charID_ = setID;																// �ő�l�ɒB���Ă����ꍇ�ŏ��l�Ɉړ�
		}
		return true;
	}
	return false;
}

// �L��������L�[���͊֐�
//-------------------------
bool SelectScene::DesideChar(Player* player, DIR dir, PL_ST state)
{
	if (player->keyNow_[static_cast<int>(dir)] && !(player->keyOld_[static_cast<int>(dir)]))		// �㉺�L�[�������ăL������ԕω�			// ��F����A���F�L�����Z��
	{
		player->state_ = state;																		// ��ԕω�
		return true;
	}
	return false;																					// �L�������㉺�L�[���������܂�DesideChar�͏�������K�v���Ȃ��̂�false��Ԃ�
}

// SelestScene�̕`��
//--------------------
void SelectScene::Draw(void)
{
	TRACE("SelectScene_Draw\n");

	DrawGraph(0, 0, bgImage_, true);																		// �Q�[���w�i�̕`��
	DrawGraph((screenSize_.x - CHAR_SEL_IMAGE_SIZE_X) / 2, 0, logoImage_, true);							// �L�����I�𒆂̃��S�̕`��
	for (int no = 0; no < PLAYER_MAX; no++)																	// �v���C���[���Ƃ̑I�𒆃L����
	{
		DrawGraph(DRAW_OFFSET_X + player[no].pos_.x, DRAW_OFFSET_Y + player[no].pos_.y - CHIP_SIZE / 3, charImage[static_cast<int>(player[no].charID_)].chipImage[static_cast<int>(DIR::DOWN)][0], true);
	}
}

// SelestScene�̏�����
//----------------------
bool SelectScene::Init(void)
{
	TRACE("SelectScene_Init\n");

	bool rtnFlag = true;
	for (int j = 0; j < static_cast<int>(CHAR_ID::MAX); j++)				// �I��p�̃L����
	{
		charImage[j].Init(static_cast<CHAR_ID>(j));
	}
	if ((bgImage_ = LoadGraph("Resource/image/bg.png")) == -1)				// �Q�[���w�i
	{
		TRACE("�ǂݍ��݃G���[\n");
		rtnFlag = false;
	}
	if ((logoImage_ = LoadGraph("Resource/image/char_sel.png")) == -1)		// �L�����Z���N�g���S
	{
		TRACE("�ǂݍ��݃G���[\n");
		rtnFlag = false;
	}
	for (int no = 0; no < PLAYER_MAX; no++)									// �L�������Ƃ̏��
	{
		player[no].state_ = PL_ST::NON;
	}
	GetScreenState(&screenSize_.x, &screenSize_.y, nullptr);				// �X�N���[���T�C�Y�̎擾
	return rtnFlag;
}

// SelestScene�̉��
//--------------------
bool SelectScene::Relese(void)
{
	TRACE("SelectScene_Relese\n");

	for (int j = 0; j < static_cast<int>(CHAR_ID::MAX); j++)				// charImage�̉��
	{
		charImage[j].Release();
	}

	player[0].Relese();					// player�̉��// �����I�ɃL�������𑝂₷�Ȃ炱����for���ł܂킷
	player[1].Relese();					// player�̉��

	DeleteGraph(logoImage_);			// �L�����Z���N�g���S�̉��
	DeleteGraph(bgImage_);				// �Q�[���w�i�̉��

	return true;
}
