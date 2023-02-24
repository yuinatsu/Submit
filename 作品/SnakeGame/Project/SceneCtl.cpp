#include <DxLib.h>
#include "SceneCtl.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

// SceneCtl�̐���
//-----------------
SceneCtl::SceneCtl()
{
	Init();								// Init�̌Ăяo��
}

// SceneCtl�̔j��
//-----------------
SceneCtl::~SceneCtl()
{
}

// GAME MAIN
//------------
void SceneCtl::Run(void)
{
	// �Q�[�����[�v
	//---------------
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		oldScnID_ = scnID_;					// ���݂�ID���ŏ���old�ɑޔ������Ă���
		if (scnID_ != nextScnID_)			// ���݂�ID�Ǝ���ID���Ⴄ�Ȃ�
		{
			SetupScene();					// �V�[���̃Z�b�g�A�b�v
		}
		nextScnID_ = scnID_;				// ����ID�����݂�ID�Ɋi�[

		GetKeyData();						// �L�[�f�[�^�擾
		Update();							// Update�̌Ăяo��

		Draw();								// Draw�̌Ăяo��
	}
	gameScene_.Relese();					// gameScene��Relese���Ăяo��
}

// SceneCtl�̏�����
//-------------------
bool SceneCtl::Init(void)
{
	SysInit();										// �V�X�e���n������
	oldScnID_ = SCN_ID::MAX;						// oldScnID�̏���ID
	scnID_ = SCN_ID::MAX;							// scnID�̏���ID
	nextScnID_ = SCN_ID::TITLE;						// nextScnID�̏���ID

	return true;
}

// �V�X�e���n������
//-------------------
bool SceneCtl::SysInit(void)
{
	// �V�X�e������
	//-------------------
	SetWindowText("�X�l�[�N�Q�[��");						// �Q�[���E�B���h�E�̃^�C�g��
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);			// �Q�[���E�B���h�E�̃T�C�Y�ƐF���[�h��ݒ�
	ChangeWindowMode(true);									// �Q�[���E�B���h�E�̕\�����@ (true : window     false : �t���X�N���[��)

	if (DxLib_Init() == -1)									// DX���C�u�����̏���������		Init:�C�j�V�����C�Y
	{
		TRACE("DxLib �̏��������s");
		return false;										// DX���C�u�����̏��������s�̈׃V�X�e���I��
	}

	srand(256);						// ��������

	return true;
}

// SceneCtl�̉��
//-----------------
bool SceneCtl::Relese(void)
{
	DxLib_End();		// DX���C�u�����̏I������
	return true;
}

// SceneCtl�̍X�V
//------------------
void SceneCtl::Update(void)
{
	switch (scnID_)
	{
	case SCN_ID::TITLE:			// �^�C�g���̍X�V
		nextScnID_ = titleScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::RULE:			// ���[���V�[���̍X�V
		nextScnID_ = ruleScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::SELECT:		// �Z���N�g�̍X�V
		nextScnID_ = selectScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::GAME:			// �Q�[���̍X�V
		nextScnID_ = gameScene_.Update(keyBuf_, keyBufOld_);
		break;
	case SCN_ID::RESULT:		// ���U���g�̍X�V
		nextScnID_ = resultScene_.Update(keyBuf_, keyBufOld_);
		break;
	default:
		TRACE("�ُ�\n");
		scnID_ = SCN_ID::TITLE;
		break;
	}
}

// SceneCtl�̕`��
//-----------------
void SceneCtl::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);			// �`�悷���ʂ𗠂̉�ʂɐݒ�
	ClearDrawScreen();						// �`�悵����ʂ��N���A

	switch (scnID_)
	{
	case SCN_ID::TITLE:			// �^�C�g���̕`��
		titleScene_.Draw();
		break;
	case SCN_ID::RULE:			// ���[���V�[���̕`��
		ruleScene_.Draw();
		break;
	case SCN_ID::SELECT:		// �Z���N�g�̕`��
		selectScene_.Draw();
		break;
	case SCN_ID::GAME:			// �Q�[���̕`��
		gameScene_.Draw();
		break;
	case SCN_ID::RESULT:		// ���U���g�̕`��
		resultScene_.Draw();
		break;
	default:
		TRACE("�ُ�\n");
		scnID_ = SCN_ID::TITLE;
		break;
	}

	ScreenFlip();
}

// �L�[�f�[�^�擾
//-----------------
void SceneCtl::GetKeyData(void)
{
	memcpy(keyBufOld_, keyBuf_, sizeof(keyBufOld_));			// keyBuf��keyBufOld�ɃR�s�[
	GetHitKeyStateAll(keyBuf_);									// �S�ẴL�[�̏�Ԃ�keyBuf�Ɋi�[
}

// �V�[���̃Z�b�g�A�b�v
//-----------------------
void SceneCtl::SetupScene(void)
{
	ReleseScene();							// �O�V�[���̉��
	CHAR_ID winnerID = CHAR_ID::DRAW;		// ������ID�̏���ID

	switch (nextScnID_)
	{
	case SCN_ID::TITLE:			// �^�C�g���̏�����
		titleScene_.Init();
		break;
	case SCN_ID::RULE:			// ���[���V�[���̏�����
		ruleScene_.Init();
		break;
	case SCN_ID::SELECT:		// �Z���N�g�̏�����
		selectScene_.Init();
		break;
	case SCN_ID::GAME:			// �Q�[���̏�����
		gameScene_.Init(selectScene_.player[0].charID_, selectScene_.player[1].charID_);						// Init(1P�̃L����ID, 2P�̃L����ID)
		break;
	case SCN_ID::RESULT:		// ���U���g�̏�����
		for (int no = 0; no < PLAYER_MAX; no++)							// �v���C���[���Ƃ̏�Ԃ��m�F
		{
			if (gameScene_.player[no].state_ == PL_ST::ALIVE)			// �����҂̃L����ID���擾
			{
				winnerID = gameScene_.player[no].charID_;
			}
		}
		resultScene_.Init(winnerID);									// ���������L������ID��Ԃ�
		break;
	default:
		TRACE("�ُ�\n");
		scnID_ = SCN_ID::TITLE;
		titleScene_.Init();
		break;
	}
	scnID_ = nextScnID_;
}


// �O�V�[���̉��
//-----------------
void SceneCtl::ReleseScene(void)
{
	switch (scnID_)
	{
	case SCN_ID::TITLE:			// �^�C�g���̉��
		titleScene_.Relese();
		break;
	case SCN_ID::RULE:			// ���[���V�[���̉��
		ruleScene_.Relese();
		break;
	case SCN_ID::SELECT:		// �Z���N�g�̉��
		selectScene_.Relese();
		break;
	case SCN_ID::GAME:			// �Q�[���̉��
		gameScene_.Relese();
		break;
	case SCN_ID::RESULT:		// ���U���g�̉��
		resultScene_.Relese();
		break;
	case SCN_ID::MAX:
		// �N�����̂ݗ���\������
		break;
	default:
		TRACE("�ُ�\n");
		break;
	}
}
