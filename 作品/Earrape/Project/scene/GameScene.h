#pragma once
#include <memory>
#include<random>
#include "BaseScene.h"
#include "../SCN_ID.h"
#include "../input/Input.h"
#include "WeaponScene.h"
#include "../common/Vector2.h"
#include"../common/Geometry.h"

#define WINDOW_SPACE (20)	// WIN�E�B���h�E�ƕ���E�B���h�E�̊Ԋu
#define ICON_SPACEX (48)	// �A�C�R�����m��X���Ԋu		//�ǔ��A�C�R�����}�E�X���킹��
#define ICON_SPACEY (32)	// �A�C�R�����m��Y���Ԋu

#define lpGameScene GameScene::GetInstance()
constexpr float LimitTime = 30.0f;
//�摜�̑傫����`
constexpr int ScoreSizeX = 190;
constexpr int ScoreSizeY = 30;
constexpr int SecSizeX = 80;
constexpr int SecSizeY = 25;
constexpr int TimeSizeX = 150;
constexpr int TimeSizeY = 30;

//�m�h���̒�`
constexpr float NodonSpeed_ = 5.0f;

class SaveLoad;
class WeaponScene;
class PerformScene;

struct Nodon
{
	Position2 pos;
	Position2 goal;
	Position2 vel;
	float Accel = 0.0f;
	bool isActive = false;
};

class GameScene :
    public BaseScene
{
public:
	static GameScene& GetInstance(void)
	{
		static GameScene s_Instance;
		return s_Instance;
	}
	GameScene();
	~GameScene();

	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void Draw(void) override;
	bool Init(void) override;
	bool Release(void) override;
	Vector2 scrSize_;	// ��ʃT�C�Y
private:
	bool pageflag_;
	int pageCnt_;			// �y�[�W�J�E���^�[
	int guideImage_[3];

	void AchievementScore(void);
	void AchievementClick(void);
	void AchievementEar(void);
	void AchievementTotalScore(void);
	void AchievementNum(void);


	void DrawNumber(float remainTime);	//�����̕`��
	void DrawEffect(int num);			//�G�t�F�N�g�̕`��
	void DrawGuide(void);				//�K�C�h�̕`��
	void DrawPause(void);				//�|�[�Y�̕`��
	void DrawIcon(Vector2 iconSpace,Vector2 iconStart);				//�A�C�R���̕`��

	int WeaponImage_[static_cast<int> (WEAPON::MAX)];	//����̉摜������Ƃ���Benum�̂l�`�w������̐�
	int IconImage_[static_cast<int> (WEAPON::MAX)];
	int effectImage_[static_cast<int> (WEAPON::MAX)];
	int iconoffset;
	int BackGroundImage_;	//�w�i 

	int mimiSpd_;

	int atfSE_;		// A.T�t�B�[���h��SE
	int atkSE_;
	int explosionSE_;
	int destuctionSE_;		//��
	int click_SE_;
	int BERA_SE_;
	int brooming_SE_;
	int omoriSE_;
	int himei_SE_;

	int white_;
	int black_;
	int silver_;
	int dmgCnt_;			// �U�������Ƃ��̃��A�N�V�����p�J�E���^�[ ��������
	int wepNum_;			// ����̃i���o�[
	bool wepFlag_;			// ����I��p�t���O
	bool atkflag_;			// �U���t���O
	Vector2 drawOffset_;	// ���o�p�@��������
	Vector2 pos_;			//�}�E�X�̍��W�@�Q�[���p�H

	// �Z�[�u��̏��
	int wepOpen_[static_cast<int> (WEAPON::MAX)] = { 1,1,1,1,1,1 };			// ����J���󋵁i�����̊J���󋵂�2�j
	int ptrOpen_[static_cast<int> (PERFORM::MAX)] = 
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1};				// ���ъJ����
	int WepCnt_[static_cast<int> (WEAPON::MAX)];					// ���킲�Ƃ̃N���b�N�J�E���g
	int MimiCnt_[static_cast<int> (CHARACTER::MAX)];				// �����Ƃ̃N���b�N�J�E���g
	int ptrCnt;														// ���ъJ����
	int WepMax;														// �g�p�ł��镐��̐�

	Nodon nodon_[16];			//�m�h��
	std::random_device rnd;		//����

	//���Ԋ֌W
	int StartTime;
	int EndTime;	//�I������
	int aftime;		//�o�ߎ���
	int Alltime;	//�Q�[���̍��v�v���C����
	bool timeflag_;		//���Ԓ�~�t���O
	bool Limitflag;		//�������Ԃ�����n�߂�t���O
	//int pusReturnTime;	//��~���Ԃ̕ێ�

	float cntdow;

	//�t�H���g�摜�W
	int ScoreImage_;			//SCORE�̃n���h��
	int SecImage_;				//sec�̃n���h��
	int TimeImage_;				//TIME�̃n���h��
	
	int himeiCnt;
	int himeilimit;

	bool effectflag;
	int effectCnt_;	//�G�t�F�N�g�̎���
	Vector2 posefct;

	int  AbortBO_, AbortBW_, section1_BO_, section1_BW_, section2_BO_, section2_BW_;
	bool sctFlag1_, sctFlag2_, sctFlag3_;

	friend SaveLoad;
	friend WeaponScene;
	friend PerformScene;
};

