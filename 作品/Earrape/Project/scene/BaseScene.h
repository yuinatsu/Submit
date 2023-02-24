#pragma once
#include <memory>
#include <list>
#include "../common/SaveLoad.h"
#include "../common/DataID.h"
#include "../SCN_ID.h"

class BaseScene;
using uniqueBaseScene = std::unique_ptr<BaseScene>;

constexpr int AtkPoint = 1000;										//����̊�b�U���́i���j
constexpr int FontOffset = 120;										//�t�H���g�̃I�t�Z�b�g�i���j
constexpr int LimitTimeDigit = 2;									//�������Ԃ̌����i�L���͊܂܂Ȃ�)
constexpr int ScoreDigit = 7;										//�X�R�A�̌���

//�e�t�H���g�̑傫����`
constexpr int FontNumSizeX = 60;									//�����̃t�H���g�T�C�Y�w
constexpr int FontNumSizeY = 60;									//�����̃t�H���g�T�C�Y�x
constexpr int TotalLimitTimeSizeX = FontNumSizeX * LimitTimeDigit;	//�������Ԃ̃T�C�Y���v�w
constexpr int TotalScoreSizeX = FontNumSizeX * ScoreDigit;			//�X�R�A�̃T�C�Y���v�w
constexpr int EarSizeX = 205;										//���̑傫���w
constexpr int EarSizeY = 300;										//���̑傫���x 

class BaseScene
{
public:
	BaseScene() {};
	virtual ~BaseScene() {};
	virtual SCN_ID Update(char* keyData, char* keyDataOld) = 0;		// �X�V(���݂̃L�[���, �ߋ��̃L�[���)
	virtual void Draw() = 0;										// �`��
	virtual bool Init(void) = 0;									// ������
	virtual bool Release(void) = 0;									// ���
};
