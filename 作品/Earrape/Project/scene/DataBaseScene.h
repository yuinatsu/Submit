#pragma once
#include "BaseScene.h"
#include "../common/Vector2.h"

#define lpDataBaseScene DataBaseScene::GetInstance()

class DataBaseScene :
	public BaseScene
{
public:
	static DataBaseScene& GetInstance(void)
	{
		static DataBaseScene s_Instance;
		return s_Instance;
	}
	DataBaseScene();
	~DataBaseScene();
	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void Draw(void) override;	// �O���t�B�b�N�̕`��
	bool Init(void) override;	// ������
	bool Release(void) override;	// ���\�[�X�̉��
private:
	int dbLogoBO_, dbLogoBW_, section1_BO_, section1_BW_, section2_BO_, section2_BW_, dbbg_;			// �w�i�A���S�A���̑��`���

	int atfSE_;						// SE
	bool sctFlag1_, sctFlag2_;		// �I���t���O

	int blinkCnt_;					// �_��

	Vector2 pos_;					// ���W
};

