#pragma once
#include <DxLib.h>
#include "MapCtl.h"
#include "../common/ImageMng.h"
#include "../common/Vector2.h"


// �W���`��֐�
//---------------
struct DrawChipDef
{
	void operator()(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)			// drawOffset�̓}�X�ڐ�
	{
		DrawGraph(x * chipSize.x + drawOffset.x, y * chipSize.y + drawOffset.y, imageID, true);
	}
};


// �����`�悵�Ȃ��`��֐�
//-------------------------
struct DrawChipNon
{
	void operator()(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)
	{
		// �����`�悵�Ȃ�
	}
};


// �E�o�͂����`��֐�		������W�߂������畁�ʂ̂͂����Ƃ��ĕ`��
//---------------------
//struct DrawChipEscape
//{
//	void operator()(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)
//	{
//		if (!lpMapCtl.GetGoldCnt())
//		{
//			DrawGraph(x * chipSize.x + drawOffset.x, y * chipSize.y + drawOffset.y, lpImageMng.GetID("tileset")[static_cast<int>(ChipID::LAD)], true);
//		}
//	}
//};


// �g���b�v�`��֐�			�����ڂ͌@���u���b�N�ɂ���
//-------------------
struct DrawChipTrap
{
	void operator()(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)
	{
		DrawGraph(x * chipSize.x + drawOffset.x, y * chipSize.y + drawOffset.y, lpImageMng.GetID("tileset")[static_cast<int>(ChipID::BBL)], true);
	}
};


// �󂹂�u���b�N�`��֐�
//-------------------------
//#define DigCnt 10														// �P�R�}������̃t���[����
//#define DigFrameLim 5													// �R�}��
//#define DigWait (60 * 3)												// 3�b�Ԃ̑ҋ@����
//#define DigFullFrame ((DigCnt * DigFrameLim) * 2 + DigWait)			// ���t���[����
struct DrawChipBbl
{
	void operator()(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)
	{
		// �����@��ۂ̍��ڂ���G�t�F�N�g�����\��
		int id = 0;
		int count = lpMapCtl.GetDigData(x, y);
		if ((count / DigCnt) == 0)
		{
			id = imageID;
		}
		else
		{
			if (count > ((DigCnt * DigFrameLim) + DigWait))					// ���@��p�[�g						// DigFullFrame ... (DigCnt * DigFrameLim) -> DigWait -> (DigCnt * DigFrameLim) ...
			{
				int point = ((DigFullFrame - count) / DigCnt) * static_cast<int>(ChipID::MAX);										// (DigFullFrame - count) ... �����l�O����J�E���g�A�b�v�����邽�߂ɂ���			DigCnt * static_cast<int>(ChipID::MAX) ... �P�O���݂���Ȃ��P�P���݂�����
				id = lpImageMng.GetID("tileset")[static_cast<int>(ChipID::BBL) + point];						// �A�j���[�V��������i�ނ��Ƃ�11���Z�����
			}
			if (count <= (DigCnt * DigFrameLim))							// ���C���p�[�g
			{
				int point = (DigFrameLim - ((DigCnt * DigFrameLim) - count) / DigCnt) * static_cast<int>(ChipID::MAX) + 1;			// DigFrameLim���������MAX����O�ɋ߂Â��Ă���		((DigCnt * DigFrameLim) - count) ... �����l�O����J�E���g�A�b�v�����邽�߂ɂ���
				id = lpImageMng.GetID("tileset")[static_cast<int>(ChipID::BBL) + point];
			}
		}
		if (id)
		{
			DrawGraph(x * chipSize.x + drawOffset.x, y * chipSize.y + drawOffset.y, id, true);
		}
	}
};
