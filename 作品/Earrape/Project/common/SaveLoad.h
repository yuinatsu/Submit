#pragma once
#include "../common/DataID.h"

#define lpSaveLoad SaveLoad::GetInstance()

// �Z�[�u�p�f�[�^
enum class SaveData
{
	wep,			// ����J����
	ptr,			// ���ъJ����
	wepClick,		// ����N���b�N��
	mimiClick,		// ���N���b�N��
	score,			// �g�[�^���X�R�A
	MAX
};

class SaveLoad
{
public:
	static SaveLoad& GetInstance(void)
	{
		static SaveLoad s_Instance;
		return s_Instance;
	}
	SaveLoad() {};
	~SaveLoad() {};
	void DataSave();			// �f�[�^�Z�[�u
	void DataLoad();			// �f�[�^���[�h

	int DataList[static_cast<int>(SaveData::MAX)][static_cast<int>(PERFORM::MAX)];			// �f�[�^�z��

	int wepOpen[static_cast<int> (WEAPON::MAX)] = {1,1};		// ����J����
	int ptrOpen[static_cast<int> (PERFORM::MAX)] = {};			// ���ъJ����
	int WepCnt[static_cast<int> (WEAPON::MAX)] = {};			// ���킲�Ƃ̃N���b�N�J�E���g
	int MimiCnt[static_cast<int> (CHARACTER::MAX)] = {};		// �����Ƃ̃N���b�N�J�E���g
	int ScoreCnt;												// �g�[�^���̃X�R�A�J�E���g
};

//SaveLoad�œ�̗�O�X���[���o���ꍇ
// SceneCtl::Draw�̓����ɂ���_dbgStartDraw()�ŗ�O�X���[����������ꍇ������܂��B
// �Z�[�u���邽�߂̕ϐ��̌^���ASaveLoad�̓����ɂ���fread(&DataList, sizeof(int), 256, fp)��"sizeof(int)"�̌^�ɍ��킹��ΑΏ��\�ł��B
