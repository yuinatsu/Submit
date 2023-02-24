#pragma once
#include "Player.h"
#include "Vector2.h"
#include "MapCtl.h"
#include "SCN_ID.h"
#include "common.h"

#define TITLE_IMAGE_SIZE_X 720							// �^�C�g�����S���T�C�Y
#define TITLE_IMAGE_SIZE_Y 350							// �^�C�g�����S�c�T�C�Y

#define START_COUNT_MAX 60								// �_�ŃJ�E���g����

class TitleScene
{
public:
	TitleScene();												// TitleScene�̐���
	~TitleScene();												// TitleScene�̔j��
	SCN_ID Update(char* keyData, char* keyDataOld);				// TitleScene�̍X�V				// char   �ϐ��n��      char*   �|�C���^�n���@�@�@char&   �Q�Ɠn��(���̂��K�v)...�Q�Ƃ̔z��͎g���Ȃ�
	void Draw(void);											// TitleScene�̕`��
	bool Init(void);											// TitleScene�̏�����
	bool Relese(void);											// TitleScene�̉��
private:
	Vector2 screenSize_;								// �X�N���[���T�C�Y
	int blinkCnt_;										// �_�ŃJ�E���^
	int TitleBgImage_;									// �^�C�g���w�i
	int TitleImage_;									// �^�C�g�����S
	int SpaceImage_;									// "PUSH SPACE KEY"
	bool startFlag_ = false;							// �_�ŃX�s�[�h�t���O
	int startCnt_;										// �_�ŃX�s�[�h�J�E���^
};

