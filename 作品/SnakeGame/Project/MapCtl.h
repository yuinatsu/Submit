#pragma once
#include "common.h"
#include "Vector2.h"

#define BLOCK_PTN_MAX 7															// �󔒂��܂ރu���b�N�̃p�^�[����
#define BLOCK_CNT_INV 3															// �u���b�N�̃T�C�Y�A�b�v����t���[���C���^�[�o��
#define BLOCK_MAX_SIZE_CNT ((BLOCK_PTN_MAX - 1) * BLOCK_CNT_INV)				// �u���b�N���ő�T�C�Y�ɂȂ�Ƃ��̃J�E���g��

class MapCtl
{
public:
	MapCtl();															// MapCtl�̐���
	~MapCtl();															// MapCtl�̔j��
	void Update(void);													// MapCtl�̍X�V
	void Draw(void);													// MapCtl�̕`��
	bool Init(void);													// MapCtl�̏�����
	bool Relese(void);													// MapCtl�̉��
	int SetBlock(Vector2 pos, int cnt = 1);								// �u���b�N�̔z�u			// SetBlock(�v���C���[���W, �J�E���g)
	int GetBlock(Vector2 pos);											// �}�b�v�͈̓`�F�b�N		// GetBlock(�v���C���[���W)
	bool isOnBlock(Vector2 pos);										// �u���b�N�T�C�Y�`�F�b�N	// isOnBlock(�v���C���[���W)
private:
	int blockImage[BLOCK_PTN_MAX];								// �u���b�N�摜
	int mapData[MAP_CHIP_COUNT_Y][MAP_CHIP_COUNT_X];			// �}�b�v�T�C�Y
};

