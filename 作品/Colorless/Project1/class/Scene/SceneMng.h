#pragma once
#include <array>
#include <memory>
#include <chrono>
#include "BaseScene.h"
#include "../input/Controller.h"
#include "../common/Vector2.h"

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng& GetInstance(void) 
	{
		static SceneMng s_Instance;
		return s_Instance;
	}
	void Run(void);											// ���C�����[�v
	void Update(void);										// �X�V
	void Draw(void);										// �`��
	const Vector2 GetScreenSize(void)const;					// ��ʃT�C�Y�擾
	Stage GetStage(void);									// �X�e�[�W���擾
	void SetStage(int num);									// �X�e�[�W���Z�b�g
	char keyBuf_[256];										// ���݂̃L�[�o�b�t�@
	char keyBufOld_[256];									// �P�O�̃L�[�o�b�t�@
	CntType GetCntType(void);
	void SetCntType(CntType cntType);
private:
	SceneMng();
	~SceneMng();
	bool SysInit(void);										// �V�X�e���̏�����
	bool initFlag_;											// �������p�t���O
	uniqueBaseScene scene_;									// �V�[���̊i�[��
	const Vector2 screenSize_;								// �X�N���[���T�C�Y
	Stage stage_;											// �X�e�[�W���i�[�p
	CntType cntType_;
};