#include <DxLib.h>
#include "ResultScene.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

#define WIN_MES_IMAGE_SIZE_X 400													// �������b�Z�[�W�摜�̉��T�C�Y
#define WIN_MES_IMAGE_SIZE_Y (300 / static_cast<int>(CHAR_ID::MAX))					// �������b�Z�[�W�摜�̏c�T�C�Y

// ResultScene�̐���
//--------------------
ResultScene::ResultScene()
{
	TRACE("ResultScene�̐���\n");
}

// ResultScene�̔j��
//--------------------
ResultScene::~ResultScene()
{
	TRACE("ResultScene�̔j��\n");
}

// ResultScene�̍X�V
//--------------------
SCN_ID ResultScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("ResultScene_Update\n");

	if (finishCnt_ < FINISH_COUNT_MAX)					// ��萔�J�E���g���s���Ă����RESULT�ɕԂ葱����
	{
		finishCnt_++;
		return SCN_ID::RESULT;
	}

	if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])				// SPACE�������ă^�C�g���ɖ߂�
	{
		return SCN_ID::TITLE;
	}
	if (keyData[KEY_INPUT_F12] && !keyDataOld[KEY_INPUT_F12])					// �����^�C�g���ړ�
	{
		return SCN_ID::TITLE;
	}
	return SCN_ID::RESULT;
}

// ResultScene�̕`��
//--------------------
void ResultScene::Draw(void)
{
	TRACE("ResultScene_Draw\n");

	DrawGraph(0, picPos_.y + CHAR_PIC_SIZE_Y / 3, winMesImage[static_cast<int>(winnerID_)], true);				// �����҃��b�Z�[�W			// bgImage����ɋL�q���邱�Ƃɂ���ăJ�E���g���I���܂ł͕\������Ȃ��悤�Ɍ�����

	switch (winnerID_)																							// �L���������G
	{
	case CHAR_ID::REIMU:
		DrawGraph(picPos_.x, picPos_.y, charImage_.picImage[0][0], true);
		break;
	case CHAR_ID::MARISA:
		DrawGraph(picPos_.x, picPos_.y, charImage_.picImage[0][1], true);
		break;
	case CHAR_ID::CIRNO:
		DrawGraph(picPos_.x, picPos_.y, charImage_.picImage[0][2], true);
		break;
	case CHAR_ID::SAKUYA:
		DrawGraph(picPos_.x, picPos_.y, charImage_.picImage[0][3], true);
		break;
	case CHAR_ID::YOUMU:
		DrawGraph(picPos_.x, picPos_.y, charImage_.picImage[0][4], true);
		break;
	case CHAR_ID::DRAW:
		break;
	default:
		break;
	}

	if ((blinkCnt_ / 50) % 2)													// �_��
	{
		DrawExtendGraph(spacePos_.x, spacePos_.y, spacePos_.x + SPACE_IMAGE_SIZE_X / 2, spacePos_.y + SPACE_IMAGE_SIZE_Y / 2, SpaceImage_, true);				// "PUSH SPACE KEY"(�k��)
	}
	blinkCnt_++;

	if (!(finishCnt_ >= FINISH_COUNT_MAX))																		// ��萔�Q�[���w�i��\��
	{
		if (finishCnt_ > 0)
		{
			DrawGraph(0, 0, bgImage_, true);		// �w�i�`��
		}
	}
}

// ResultScene�̏�����
//----------------------
bool ResultScene::Init(CHAR_ID winnerID)
{
	TRACE("ResultScene_Init\n");

	GetDrawScreenSize(&screenSize_.x, &screenSize_.y);						// �X�N���[���`����擾
	bgImage_ = MakeScreen(screenSize_.x, screenSize_.y, false);				// bgImage�Ɋi�[
	GetDrawScreenGraph(0, 0, screenSize_.x, screenSize_.y, bgImage_);		// �Q�[���w�i�̕`��

	charImage_.Init(winnerID);													// charImage��Init���Ăяo��

	bool rtnFlag = true;
	if ((LoadDivGraph("Resource/image/win.png", static_cast<int>(CHAR_ID::MAX), 1, static_cast<int>(CHAR_ID::MAX), WIN_MES_IMAGE_SIZE_X, WIN_MES_IMAGE_SIZE_Y, winMesImage)) == -1)				// �����҃��b�Z�[�W
	{
		TRACE("�ǂݍ��݃G���[\n");
		rtnFlag = false;
	}
	if ((SpaceImage_ = LoadGraph("Resource/image/space.png")) == -1)			// "PUSH SPACE KEY"�̓ǂݍ���
	{
		TRACE("�ǂݍ��݃G���[\n");
		rtnFlag = false;
	}

	GetScreenState(&screenSize_.x, &screenSize_.y, nullptr);					// �X�N���[���T�C�Y�̎擾

	picPos_.x = (screenSize_.x - CHAR_PIC_SIZE_X) * 5 / 6;						// �L���������GX���W
	picPos_.y = (screenSize_.y - CHAR_PIC_SIZE_Y) / 2;							// �L���������GY���W

	spacePos_.x = (screenSize_.x - SPACE_IMAGE_SIZE_X) * 2 / 5;					// "PUSH SPACE KEY"��X���W
	spacePos_.y = (screenSize_.y - SPACE_IMAGE_SIZE_Y) * 3 / 5;					// "PUSH SPACE KEY"��Y���W

	finishCnt_ = 0;																// ���}�����^���̏�����

	winnerID_ = winnerID;														// gameScene���瑗���Ă���winnerID��winnerID_�Ɋi�[

	return rtnFlag;
}

// ResultScene�̉��
//--------------------
bool ResultScene::Relese(void)
{
	TRACE("ResultScene_Relese\n");

	DeleteGraph(SpaceImage_);													// "PUSH SPACE KEY"�̉��

	for (int no = 0; no < static_cast<int>(CHAR_ID::MAX); no++)					// �����҃��b�Z�[�W�̉��
	{
		DeleteGraph(winMesImage[no]);
	}

	for (int no = 0; no < PLAYER_MAX; no++)										// player�̉��
	{
		player[no].Relese();
	}

	DeleteGraph(bgImage_);														// �Q�[���w�i

	return true;
}
