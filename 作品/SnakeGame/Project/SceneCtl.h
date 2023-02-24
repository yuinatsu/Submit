#pragma once
#include "TitleScene.h"
#include "RuleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "SCN_ID.h"

#define SCREEN_SIZE_X 800		// ��ʂ̉��T�C�Y
#define SCREEN_SIZE_Y 600		// ��ʂ̏c�T�C�Y

class SceneCtl
{
public:
	SceneCtl();								// SceneCtl�̐���
	~SceneCtl();							// SceneCtl�̔j��
	void Run(void);							// GAME MAIN
private:
	bool Init(void);						// SceneCtl�̏�����
	bool SysInit(void);						// �V�X�e���n������
	bool Relese(void);						// SceneCtl�̉��
	void Update(void);						// SceneCtl�̍X�V
	void Draw(void);						// SceneCtl�̕`��

	void GetKeyData(void);					// �L�[�f�[�^�擾

	void SetupScene(void);					// �V�[���̃Z�b�g�A�b�v
	void ReleseScene(void);					// �O�V�[���̉��

	TitleScene titleScene_;			// TitleScene�̏��Ǘ�
	RuleScene ruleScene_;			// RuleScene�̏��Ǘ�
	SelectScene selectScene_;		// SelectScene�̏��Ǘ�
	GameScene gameScene_;			// GameScene�̏��Ǘ�
	ResultScene resultScene_;		// ResultScene�̏��Ǘ�

	SCN_ID scnID_;					// ���݂�SCN_ID
	SCN_ID oldScnID_;				// �P�O��SCN_ID
	SCN_ID nextScnID_;				// �P���SCN_ID
	char keyBuf_[256];				// ���݂̃L�[�o�b�t�@
	char keyBufOld_[256];			// �P�O�̃L�[�o�b�t�@
};

