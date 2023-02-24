#include <DxLib.h>
#include "TitleScene.h"
#include "_debug/_DebugConOut.h"

// TitleScene�̐���
//-------------------
TitleScene::TitleScene()
{
	TRACE("TitleScene�̐���\n");
}

// TitleScene�̔j��
//-------------------
TitleScene::~TitleScene()
{
	TRACE("TitleScene�̔j��\n");
}

// TitleScene�̍X�V
//-------------------
SCN_ID TitleScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("TitleScene_Update\n");
	
	if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])			// SPACE�L�[��������startFlag��true�ɂ���				// if(startFlag_ == true)�̒��g�����̒��ŏ�����SPACE�L�[������������K�v���o�Ă���
	{
		startFlag_ = true;
	}
	if (startFlag_ == true)													// SPACE�L�[��������startFlag��true�ɂȂ�����"PUSH SPACE KEY"�̍����_�ł��J�n
	{
		if (startCnt_ < START_COUNT_MAX)									// �J�E���g����萔���Z
		{
			startCnt_++;
			return SCN_ID::TITLE;											// ��萔�̃J�E���g�𒴂���܂ł�ID��TITLE�ŉ�
		}
		startFlag_ = false;
		return SCN_ID::RULE;												// �����_�ł��I�����烋�[����ʂɈړ�
	}
	return SCN_ID::TITLE;
}

// TitleScene�̕`��
//-------------------
void TitleScene::Draw(void)
{
	TRACE("TitleScene_Draw\n");

	DrawGraph(0, 0, TitleBgImage_, true);											// �^�C�g���w�i�̕`��
	DrawGraph((screenSize_.x - TITLE_IMAGE_SIZE_X) / 2, 0, TitleImage_, true);		// �^�C�g�����S�̕`��
	
	if (startFlag_ == false)														// SPACE�L�[���������܂ł͒ʏ�̑����œ_��
	{
		if ((blinkCnt_ / 50) % 2)													// �ʏ�_��
		{
			DrawGraph((screenSize_.x - SPACE_IMAGE_SIZE_X) / 2, (screenSize_.y - SPACE_IMAGE_SIZE_Y) * 2 / 3, SpaceImage_, true);			// "PUSH SPACE KEY"�̕`��
		}
	}
	else
	{
		if ((blinkCnt_ / 10) % 2)													// �����_��
		{
			DrawGraph((screenSize_.x - SPACE_IMAGE_SIZE_X) / 2, (screenSize_.y - SPACE_IMAGE_SIZE_Y) * 2 / 3, SpaceImage_, true);			// "PUSH SPACE KEY"�̕`��
		}
	}
	blinkCnt_++;																// �J�E���g�A�b�v
	
	GetScreenState(&screenSize_.x, &screenSize_.y, nullptr);					// �X�N���[���T�C�Y�̎擾
}

// TitleScene�̏�����
//---------------------
bool TitleScene::Init(void)
{
	TRACE("TitleScene_Init\n");

	bool rtnFlag = true;
	if ((TitleBgImage_ = LoadGraph("Resource/image/titlebg.jpg")) == -1)			// �^�C�g�����S�̓ǂݍ���
	{
		TRACE("�ǂݍ��݃G���[\n");
		rtnFlag = false;
	}
	if ((TitleImage_ = LoadGraph("Resource/image/title.png")) == -1)			// �^�C�g�����S�̓ǂݍ���
	{
		TRACE("�ǂݍ��݃G���[\n");
		rtnFlag = false;
	}
	if ((SpaceImage_ = LoadGraph("Resource/image/space.png")) == -1)			// "PUSH SPACE KEY"�̓ǂݍ���
	{
		TRACE("�ǂݍ��݃G���[\n");
		rtnFlag = false;
	}

	// �J�E���^�̏�����
	startCnt_ = 0;
	blinkCnt_ = 0;

	return rtnFlag;
}

// TitleScene�̉��
//-------------------
bool TitleScene::Relese(void)
{
	TRACE("TitleScene_Relese\n");

	DeleteGraph(SpaceImage_);						// "PUSH SPACE KEY"�̉��
	DeleteGraph(TitleImage_);						// �^�C�g�����S�̉��
	DeleteGraph(TitleBgImage_);						// �^�C�g���w�i�̉��

	return true;
}
