#pragma warning(disable : 4996)	// fopen���ȈՓI�Ɏg�p���Ă܂�
#include <DxLib.h>
#include "SceneCtl.h"
#include "../FilePass.h"
#include "../FileSize.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

SceneCtl::SceneCtl() :scnArea_{ 720,480 }, TotalScore_{1000}, CVS{ 50 }//,totalscore(lpGameScene.TotalScore_)
{
	SysInit();
}

SceneCtl::~SceneCtl()
{
	Release();
}

//--------------------------------------------------------------------------
//���Q�[�����[�v
//--------------------------------------------------------------------------
void SceneCtl::Run(void)
{
	Init();
	while (CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		if (ProcessMessage() != 0)
		{
			lpSaveLoad.DataSave();			// �f�[�^�Z�[�u
		}
		oldScnID_ = scnID_;
		if (scnID_ != nextScnID_)
		{
			SetupScene();	// �e�V�[����Init
		}
		nextScnID_ = scnID_;

		GetKeyData();
		Update();
		if (nextScnID_ == scnID_)
		{
			Draw();
		}
	}
	Release();
}

//--------------------------------------------------------------------------
//���V�[���̍X�V
//--------------------------------------------------------------------------
void SceneCtl::Update(void)
{
	// �V�����V�[���̍X�V
	switch (scnID_)
	{
	case SCN_ID::TITLE:
		nextScnID_ = titleScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::SELECT:
		nextScnID_ = selectScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::GAME:
		nextScnID_ = gameScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::RESULT:
		nextScnID_ = resultScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::GUIDE:
		nextScnID_ = guideScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::DATABASE:
		nextScnID_ = dataBaseScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::PERFORM:
		nextScnID_ = performScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::WEAPON:
		nextScnID_ = weaponScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::MAX:
		break;
	default:
		TRACE("���m�̃V�[��ID\n");
		scnID_ = SCN_ID::TITLE;
		break;
	}
}

//--------------------------------------------------------------------------
//���O���t�B�b�N�̕`��
//--------------------------------------------------------------------------
void SceneCtl::Draw(void)
{
	_dbgStartDraw();

	SetDrawScreen(DX_SCREEN_BACK);	// �o�b�N�o�b�t�@�ɐݒ�
	ClsDrawScreen();	// ��ʂ̏�����

	// �V�����V�[���̕`��
	switch (scnID_)
	{
	case SCN_ID::TITLE:
		titleScene_.Draw();
		break;
	case SCN_ID::SELECT:
		selectScene_.Draw();
		break;
	case SCN_ID::GAME:
		gameScene_.Draw();
		break;
	case SCN_ID::RESULT:
		resultScene_.Draw();
		break;
	case SCN_ID::GUIDE:
		guideScene_.Draw();
		break;
	case SCN_ID::DATABASE:
		dataBaseScene_.Draw();
		break;
	case SCN_ID::PERFORM:
		performScene_.Draw();
		break;
	case SCN_ID::WEAPON:
		weaponScene_.Draw();
		break;
	case SCN_ID::MAX:
		break;
	default:
		TRACE("���m�̃V�[��ID\n");
		scnID_ = SCN_ID::TITLE;
		break;
	}
	_dbgAddDraw();
	ScreenFlip();	// ����ʂ̓��e��\��ʂɔ��f������
}

//--------------------------------------------------------------------------
//��������
//--------------------------------------------------------------------------
bool SceneCtl::Init(void)
{
	SysInit();

	lpSaveLoad.DataSave();		// �Q�[�����n�߂�O�ɏ����f�[�^���Z�b�g���Ă���
	lpSaveLoad.DataLoad();

	oldScnID_ = SCN_ID::MAX;		// old�̏����l�͉��ł��������ǎ�芸����MAX�ɂ��Ă���
	scnID_ = SCN_ID::MAX;			// ���݁i�ŏ��͂܂��n�܂��ĂȂ��̂�MAX�j
	nextScnID_ = SCN_ID::TITLE;		// �\��i�ŏ��̃V�[���j

	charCnt_ = 0;

	AllScore_ = lpSaveLoad.ScoreCnt;
	
	mimiImage_[0] = LoadGraph(GH_MIMI_ISE);
	mimiImage_[1] = LoadGraph(GH_MIMI_IMIYA);
	mimiImage_[2] = LoadGraph(GH_MIMI_URAYAMA);
	mimiImage_[3] = LoadGraph(GH_MIMI_KAKITA);
	mimiImage_[4] = LoadGraph(GH_MIMI_KAZIWARA);
	himeiSE_ = LoadSoundMem(SH_HIMEI);

	//�t�H���g�摜�ǂݍ���
	constexpr int FontCntX = 10;
	constexpr int FontCntY = 1;
	LoadDivGraph(GH_LOGO_NUMBER_BW, FontCntX * FontCntY, FontCntX, FontCntY, FontNumSizeX, FontNumSizeY, NumberFontImage);

	return true;
}

//--------------------------------------------------------------------------
//���V�X�e���̏�����
//--------------------------------------------------------------------------
bool SceneCtl::SysInit(void)
{
	SetWindowIconID(103);
	SetGraphMode(scnArea_.x, scnArea_.y, 32);
	ChangeWindowMode(true);
	SetWindowText("earrape �`�ӂ߂������̖��ɂ��̈������`");
	//SetBackgroundColor(255, 255, 255);
	if (DxLib_Init() == -1)
	{
		return false;
	}
	_dbgSetup(scnArea_.x, scnArea_.y, 255);
	//srand((unsigned int)time(nullptr));
	return true;
}

//--------------------------------------------------------------------------
//�����\�[�X�̉��
//--------------------------------------------------------------------------
bool SceneCtl::Release(void)
{
	for (int num = 0; num < 10; num++)
	{
		DeleteGraph(NumberFontImage[num]);
	}

	for (int num = 0; num < 6; num++)
	{
		DeleteGraph(mimiImage_[num]);
	}

	DxLib_End();	// DX���C�u�����I������
	return true;
}

//--------------------------------------------------------------------------
//���L�[���擾
//--------------------------------------------------------------------------
void SceneCtl::GetKeyData(void)
{
	memcpy(keyBufOld_, keyBuf_, sizeof(keyBufOld_));	// 1�t���[���O�̃L�[�擾
	GetHitKeyStateAll(keyBuf_);	// ���݂̃L�[�擾
}

//--------------------------------------------------------------------------
//���e�V�[���̏�����
//--------------------------------------------------------------------------
void SceneCtl::SetupScene(void)
{
	ReleaseScene();

	// 1�t���[����̃V�[���̏�����
	switch (nextScnID_)
	{
	case SCN_ID::TITLE:
		titleScene_.Init();
		break;
	case SCN_ID::SELECT:
		selectScene_.Init();
		break;
	case SCN_ID::GAME:
		gameScene_.Init();
		break;
	case SCN_ID::RESULT:
		resultScene_.Init();
		break;
	case SCN_ID::GUIDE:
		guideScene_.Init();
		break;
	case SCN_ID::DATABASE:
		dataBaseScene_.Init();
		break;
	case SCN_ID::PERFORM:
		performScene_.Init();
		break;
	case SCN_ID::WEAPON:
		weaponScene_.Init();
		break;
	default:
		TRACE("���m�̃V�[��ID\n");
		scnID_ = SCN_ID::TITLE;
		break;
	}
	scnID_ = nextScnID_;
}

//--------------------------------------------------------------------------
//���e�V�[���̉��
//--------------------------------------------------------------------------
void SceneCtl::ReleaseScene(void)
{
	switch (oldScnID_)
	{
	case SCN_ID::TITLE:
		titleScene_.Release();
		break;
	case SCN_ID::SELECT:
		selectScene_.Release();
		break;
	case SCN_ID::GAME:
		gameScene_.Release();
		break;
	case SCN_ID::RESULT:
		resultScene_.Release();
		break;
	case SCN_ID::GUIDE:
		guideScene_.Release();
		break;
	case SCN_ID::DATABASE:
		dataBaseScene_.Release();
		break;
	case SCN_ID::PERFORM:
		performScene_.Release();
		break;
	case SCN_ID::WEAPON:
		weaponScene_.Release();
		break;
	default:
		TRACE("���m�̃V�[��ID\n");
		titleScene_.Init();
		break;
	}
}
