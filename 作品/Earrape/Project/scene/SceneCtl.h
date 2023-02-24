#pragma once
#include "TitleScene.h"
#include "SelectScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "GuideScene.h"
#include "PerformScene.h"
#include "WeaponScene.h"
#include "../common/SaveLoad.h"
#include "../common/DataID.h"
#include "../SCN_ID.h"

#define lpSceneCtl SceneCtl::GetInstance()

class SceneCtl
{
public:
	static SceneCtl& GetInstance(void)
	{
		static SceneCtl s_Instance;
		return s_Instance;
	}
	void Run(void);

	const Vector2 scnArea_;	// ��ʉ𑜓x
	//int totalscore;
	int TotalScore_;		//�������_
	int AllScore_;			//���܂łɎ擾�����S�X�R�A
	int CVS; //SE�{�����[���ݒ�

	// ��
	int charCnt_;
	int mimiImage_[5];

	//�t�H���g�摜�W
	int NumberFontImage[10];	//�����̃n���h��
private:
	SceneCtl();
	~SceneCtl();

	void Update(void);
	void Draw(void);
	bool Init(void);
	bool SysInit(void);		// �V�X�e���̏���������
	bool Release(void);
	void GetKeyData(void);
	void SetupScene(void);	// 1�t���[����̃V�[���̏�����
	void ReleaseScene(void);	// ���݂̃V�[���̉��

	TitleScene titleScene_;		// TitleScene�N���X
	SelectScene selectScene_;	// SelectScene�N���X
	GameScene gameScene_;		// GameScene�N���X
	ResultScene resultScene_;	// ResultScene�N���X
	GuideScene guideScene_;		// GuideScene�N���X
	DataBaseScene dataBaseScene_;		// DataBaseScene�N���X
	PerformScene performScene_;			// PerformScene�N���X
	WeaponScene weaponScene_;			// WeaponScene�N���X


	char keyBuf_[256];		// �L�[���͏��
	char keyBufOld_[256];	// 1�t���[���O�̃L�[���͏��
	SCN_ID scnID_;			// ���݂̃V�[��ID
	SCN_ID oldScnID_;		// 1�t���[���O�̃V�[��ID
	SCN_ID nextScnID_;		// 1�t���[����̃V�[��ID
	int himeiSE_;
};
