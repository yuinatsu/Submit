#pragma once
#include "common.h"

#define CHAR_SIZE_X 32										// �L�����T�C�Y���T�C�Y
#define CHAR_SIZE_Y 32										// �L�����T�C�Y�c�T�C�Y

#define CHAR_PIC_MAX 5										// �L������
#define CHAR_PIC_SIZE_X (1750 / CHAR_PIC_MAX)				// �L���������G�̉��T�C�Y
#define CHAR_PIC_SIZE_Y 500									// �L���������G�̏c�T�C�Y


// �L�����̕���(�摜�̕��ѕ��ɍ��킹��)
//---------------------------------------
enum class DIR
{
	DOWN,
	LEFT,
	RIGHT,
	UP,
	MAX
};

// �L�����̎��
//---------------
enum class CHAR_ID
{
	REIMU,								// �얲
	MARISA,								// ������
	CIRNO,								// �`���m
	SAKUYA,								// ���
	YOUMU,								// �d��
	DRAW,
	MAX
};

class CharImage
{
public:
	CharImage();																	// CharImage�̐���
	~CharImage();																	// CharImage�̔j��
	bool Init(CHAR_ID charID);														// CharImage�̏�����			// Init(�I���L����ID)
	bool Release(void);

	int chipImage[static_cast<int>(DIR::MAX)][ANIM_IMAGE_MAX];						// �L�����`�b�v			// chipImage[����][�A�j���[�V������]// [(unsigned int)DIR::MAX]�Ɠ��`		()�̒��Ɍ^����͂��邱�Ƃł��̒���̊֐��݂̂����̏u�Ԃ���()�̒��̌^�ɂȂ�
	int iconImage_;																	// �L�����A�C�R��
	int picImage[1][CHAR_PIC_MAX];													// �L���������G			// picImage[][�L������]

private:
	CHAR_ID charID_;																// �L����ID
};

