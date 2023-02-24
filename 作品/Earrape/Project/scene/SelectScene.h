#pragma once
#include "BaseScene.h"
#include "../common/Vector2.h"
#include"../common/Geometry.h"

constexpr float LeftEarPos_X = 110.0f;		//�ŏI�I�ȍ��̎��̈ʒuX
constexpr float CenterEarPos_X = 315.0f;	//�ŏI�I�Ȑ^�񒆂̎��̈ʒuX
constexpr float RightEarPos_X = 520.0f;		//�ŏI�I�ȉE�̎��̈ʒuX

constexpr float LREarPos_Y = 15.0f;			//�ŏI�I�ȍ��E�̎��̈ʒuY
constexpr float CenterEarPos_Y = 90.0f;		//�ŏI�I�Ȑ^�񒆂̎��̈ʒuY

constexpr float EarSpeed = 3.0f;			//�����ړ�����Ƃ��̑��x 
constexpr float EarAccel = 0.5f;			//���̉����x

constexpr int ArrowSizeX = 80;				//���̃T�C�YX
constexpr int ArrowSizeY = 35;			//�����̃T�C�YY

constexpr int StartSizeX = 120;				//�X�^�[�g�̃T�C�YX
constexpr int StartSizeY = 54;				//�X�^�[�g�̃T�C�YY


struct EarData
{
	Position2 pos;
	Position2 anotherpos;
	Vector2_2 vel;
	float Accel_ = 0.0f;
	bool MoveFlagR = false;
	bool MoveFlagL = false;
};

class SelectScene :
	public BaseScene
{
public:
	SelectScene() {
		selectbg_ = selectLogoBO_ = selectLogoBW_ = 0;
		blinkCnt_ = 0;
	};
	~SelectScene() {};
	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void EarMoveR(void);		//�E�����ւ̎��̈ړ�
	void EarMoveL(void);		//�������ւ̎��̈ړ�

	void Draw(void) override;	// �O���t�B�b�N�̕`��
	bool Init(void) override;	// ������
	bool Release(void) override;	// ���\�[�X�̉��
private:
	int pageCnt_;			// �y�[�W�J�E���^�[
	Vector2 pos_;


	EarData LeftEar;	//���݂̉E�̎��̈ʒu
	EarData CenterEar;	//���݂̐^�񒆂̎��̈ʒu
	EarData RightEar;	//���݂̍��̎��̈ʒu
	EarData AddEar;		//�ǉ�����鎨

	const Vector2 RUear = { 840 ,-EarSizeY };
	const Vector2 LUear = { -EarSizeX,-EarSizeY };
	int selectbg_;
	int selectLogoBO_;
	int selectLogoBW_;


	int blinkCnt_;

	//�n���h��
	int RightArrow_;
	int LeftArrow_;
	int StartButton_;

	int Select_SE_;
	int atfSE_;

	bool ImageChangeF_;
	bool Lflag_;
	bool Rflag_;
};

