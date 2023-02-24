#include <DxLib.h>
#include "MapCtl.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

// MapCtl�̐���
//---------------
MapCtl::MapCtl()
{
}

// MapCtl�̔j��
//---------------
MapCtl::~MapCtl()
{
}

// MapCtl�̍X�V
//---------------
void MapCtl::Update(void)
{
	for (int y = 0; y < MAP_CHIP_COUNT_Y; y++)														// �}�b�v�̏c�̃u���b�N��
	{
		for (int x = 0; x < MAP_CHIP_COUNT_X; x++)													// �}�b�v�̉��̃u���b�N��
		{
			if ((mapData[y][x] > 0) && (mapData[y][x] < BLOCK_MAX_SIZE_CNT))						// mapData[y][x]���O�ȏ�BLOCK_MAX_SIZE_CNT�����������ꍇ
			{
				mapData[y][x]++;																	// ���Z
			}
		}
	}
}

// MapCtl�̕`��
//---------------
void MapCtl::Draw(void)
{
	for (int y = 0; y < MAP_CHIP_COUNT_Y; y++)														// �}�b�v�̏c�̃u���b�N��
	{
		for (int x = 0; x < MAP_CHIP_COUNT_X; x++)													// �}�b�v�̉��̃u���b�N��
		{
			if (mapData[y][x] > 0)																	// mapData��0���傫����Ε\��
			{
				DrawGraph(DRAW_OFFSET_X + x * CHIP_SIZE, DRAW_OFFSET_Y + y * CHIP_SIZE, blockImage[(mapData[y][x] / BLOCK_CNT_INV) % BLOCK_PTN_MAX], true);					// �u���b�N			// mapData[y][x]�̐��l��0,0,0,1,1,1,2,2,2......6,6,6�܂ōs���悤�ɂ���
			}
		}
	}
}

// MapCtl�̏�����
//---------------
bool MapCtl::Init(void)
{
	bool rtnFlag = true;
	if (LoadDivGraph("Resource/image/block.png", BLOCK_PTN_MAX * 1, BLOCK_PTN_MAX, 1, CHIP_SIZE, CHIP_SIZE, blockImage) == -1)					// �u���b�N
	{
		rtnFlag = false;
	}

	memset(mapData, 0, sizeof(mapData));					// mapData���Z�b�g

	return rtnFlag;
}

// MapCtl�̉��
//---------------
bool MapCtl::Relese(void)
{
	for (int k = 0; k < BLOCK_PTN_MAX; k++)					// �u���b�N�̉��
	{
		DeleteGraph(blockImage[k]);
	}
	return true;
}

// �u���b�N�̔z�u
//-----------------
int MapCtl::SetBlock(Vector2 pos, int cnt)
{
	Vector2 chipPos;								// �}�X�ڕϊ�
	chipPos.x = pos.x / CHIP_SIZE;					// �v���C���[�̂�����W�̃u���b�N�̃}�X�ڐ�
	chipPos.y = pos.y / CHIP_SIZE;					// �v���C���[�̂�����W�̃u���b�N�̃}�X�ڐ�

	if ((chipPos.x >= MAP_CHIP_COUNT_X) || (chipPos.x < 0) || (chipPos.y >= MAP_CHIP_COUNT_Y) || (chipPos.y < 0))			// �}�b�v�͈̔̓`�F�b�N
	{
		return 0;					// �}�b�v�O�ɍs������O��Ԃ� (�}�b�v���ł����if���𔲂��ĉ��ɍs��)
	}

	if (mapData[chipPos.y][chipPos.x] == 0)				// �w��}�X�ւ̏��̏�������
	{
		mapData[chipPos.y][chipPos.x] = cnt;
	}

	return mapData[chipPos.y][chipPos.x];			// �}�b�v����chip�̍��W��Ԃ�
}

// �}�b�v�͈̓`�F�b�N
//---------------------
int MapCtl::GetBlock(Vector2 pos)
{
	Vector2 chipPos;								// �}�X�ڕϊ�
	chipPos.x = pos.x / CHIP_SIZE;					// �v���C���[�̂�����W�̃u���b�N�̃}�X�ڐ�
	chipPos.y = pos.y / CHIP_SIZE;					// �v���C���[�̂�����W�̃u���b�N�̃}�X�ڐ�

	if ((chipPos.x >= MAP_CHIP_COUNT_X) || (chipPos.x < 0) || (chipPos.y >= MAP_CHIP_COUNT_Y) || (chipPos.y < 0))			// �}�b�v�͈̔̓`�F�b�N
	{
		//TRACE("�͈͊O\n");
		return BLOCK_MAX_SIZE_CNT;										// �}�b�v�O�ɍs������u���b�N�̍ő�T�C�Y�̒l��Ԃ� (�}�b�v���ł����if���𔲂��ĉ��ɍs��)
	}

	return mapData[chipPos.y][chipPos.x];			// �}�b�v����chip�̍��W��Ԃ�
}

// �u���b�N�T�C�Y�`�F�b�N
//-------------------------
bool MapCtl::isOnBlock(Vector2 pos)
{
	GetBlock(pos);																// GetBlock(pos)���Ăяo��
	if (GetBlock(pos) >= BLOCK_MAX_SIZE_CNT)									// �u���b�N�T�C�Y���ő�l��������Atrue��Ԃ�
	{
		return true;
	}
	return false;
}
