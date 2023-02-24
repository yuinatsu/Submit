#pragma once
#include "DataBaseScene.h"
class WeaponScene :
	public DataBaseScene
{
public:
	WeaponScene();
	~WeaponScene();
	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void Draw(void) override;	// �O���t�B�b�N�̕`��
	bool Init(void) override;	// ������
	bool Release(void) override;	// ���\�[�X�̉��
private:
	int wepOpen_[static_cast<int> (WEAPON::MAX)] = { 1,1 };			// ����J���󋵁i�����̊J���󋵂�2�j
	int WepCnt_[static_cast<int> (WEAPON::MAX)];					// ���킲�Ƃ̃N���b�N�J�E���g

	int wepNum_;			// ����̃i���o�[
	bool wepFlag_;			// ����I��p�t���O

	int IconImage_[static_cast<int> (WEAPON::MAX)];
	int DataImage_[static_cast<int> (WEAPON::MAX)];

	Vector2 iconStart;
	Vector2 iconSpace;
	int WepMax;

	Vector2 pos_;
	int iconoffset;
	bool effectflag;
	int click_SE_;
};

