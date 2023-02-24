#pragma once
#include "DataBaseScene.h"
class PerformScene :
	public DataBaseScene
{
public:
	PerformScene();
	~PerformScene();
	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void Draw(void) override;	// �O���t�B�b�N�̕`��
	bool Init(void) override;	// ������
	bool Release(void) override;	// ���\�[�X�̉��
private:
	int itemNum_;			// ���ڂ̃i���o�[
	int ItemImage_[static_cast<int> (Dataitem::MAX)];
	int perImage_[static_cast<int> (PERFORM::MAX)];
	int nonImage_;

	Vector2 iconStart;
	Vector2 iconSpace;
	Vector2 perSpace;

	Vector2 pos_;
	int iconoffset;
	bool effectflag;
};

