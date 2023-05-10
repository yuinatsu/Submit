#pragma once
#include "BaseScene.h"
#include "../Common/Time.h"

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	// �ÓI��Singleton
	static SceneMng& GetInstance()
	{
		static SceneMng s_Instance;
		return s_Instance;
	}
	// ���s����
	void Run(void);
private:
	// �V�X�e���n������
	bool SysInit(void);
	// ������
	void Init(void);
	// �X�V
	void Update(void);
	// �`��
	void Draw(void);
	// �I������
	void End(void);
	// ���Ԍn�̃N���X
	TimeClass time_;
	// �V�[�����
	SceneUptr scene_;
};

