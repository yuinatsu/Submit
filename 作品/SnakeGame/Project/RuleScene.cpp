#include <DxLib.h>
#include "RuleScene.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

// RuleScene�̐���
//------------------
RuleScene::RuleScene()
{
	TRACE("RuleScene�̐���\n");
}

// RuleScene�̔j��
//------------------
RuleScene::~RuleScene()
{
	TRACE("RuleScene�̔j��\n");
}

// RuleScene�̍X�V
//------------------
SCN_ID RuleScene::Update(char* keyData, char* keyDataOld)
{
	TRACE("RuleScene_Update\n");

	if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])					// SPACE�������ƃZ���N�g��ʂɈړ�
	{
		return SCN_ID::SELECT;
	}
	if (keyData[KEY_INPUT_F12] && !keyDataOld[KEY_INPUT_F12])						// �����^�C�g���ړ�
	{
		return SCN_ID::TITLE;
	}
	return SCN_ID::RULE;
}

// RuleScene�̕`��
//------------------
void RuleScene::Draw(void)
{
	TRACE("RuleScene_Draw\n");

	if ((blinkCnt_ / 50) % 2)													// �_��
	{
		DrawExtendGraph(spacePos_.x, spacePos_.y, spacePos_.x + SPACE_IMAGE_SIZE_X / 2, spacePos_.y + SPACE_IMAGE_SIZE_Y / 2, SpaceImage_, true);				// "PUSH SPACE KEY"(�k��)
	}
	blinkCnt_++;
}

// RuleScene�̏�����
//--------------------
bool RuleScene::Init(void)
{
	TRACE("RuleScene_Init\n");

	bool rtnFlag = true;
	if ((SpaceImage_ = LoadGraph("Resource/image/space.png")) == -1)			// "PUSH SPACE KEY"�̓ǂݍ���
	{
		TRACE("�ǂݍ��݃G���[\n");
		rtnFlag = false;
	}

	GetScreenState(&screenSize_.x, &screenSize_.y, nullptr);					// �X�N���[���T�C�Y�擾

	spacePos_.x = screenSize_.x - SPACE_IMAGE_SIZE_X  * 2 / 3;					// "PUSH SPACE KEY"��X���W
	spacePos_.y = screenSize_.y - SPACE_IMAGE_SIZE_Y;							// "PUSH SPACE KEY"��Y���W

	return rtnFlag;
}

// RuleScene�̉��
//------------------
bool RuleScene::Relese(void)
{
	TRACE("RuleScene_Relese\n");

	DeleteGraph(SpaceImage_);													// "PUSH SPACE KEY"�̉��
	return true;
}
