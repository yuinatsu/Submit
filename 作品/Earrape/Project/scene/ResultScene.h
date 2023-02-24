#pragma once
#include<random>
#include "BaseScene.h"
#include"GameScene.h"

constexpr int FontOffsetX = 300;			//�󔒕����w
constexpr int FontOffsetY = 60;				//�󔒕����x
constexpr int RankResult_SizeX = 160;		//�����N�̉摜�T�C�Y�w
constexpr int RankResult_SizeY = 80;		//�����N�̉摜�T�C�Y�x
constexpr int RankResult_Cnt = 3;			//�����N�̐�
constexpr int SpeechBalloon_SizeX = 530;	//�����o���̉摜�T�C�Y�w
constexpr int SpeechBalloon_SizeY = 180;	//�����o���̉摜�T�C�Y�x
constexpr int DrumLimit = 180;				//�h�������[�����鎞�ԁi���j
constexpr int EarCommentCnt = 21;			//���̃R�����g�̐�

class ResultScene :
    public BaseScene
{
public:
	ResultScene();
	~ResultScene();

	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void Draw(void) override;		// �O���t�B�b�N�̕`��
	bool Init(void) override;		// ������
	bool Release(void) override;	// ���\�[�X�̉��

private:

	int DrumCount_;				//�h�������[���̃J�E���g 
	void DrawResult(void);		//�X�R�A�ƃ����N�̕`�� 

	//�n���h���W
	int BackGroundImage_;		//�w�i�̃n���h��
	//int SpeechBalloonImage_;	//�����o���̃n���h��

	//�t�H���g�摜�W
	int ScoreImage_;			//SCORE�̃n���h��
	int RankImage_;				//RANK�̃n���h��
	int ABCsImage_[3];			//ABC�̃n���h��

	int EarComment_[EarCommentCnt + 1];			//���̃R�����g�n���h��
	int CommentNum_;

	std::random_device rnd;		//����
	int DrumrollSE_;
	int roll_SE_;

	bool rollSEflag_;
};

