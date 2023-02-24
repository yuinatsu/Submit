#pragma once
#include <algorithm>
#include "../Obj/Obj.h"
#include "../SceneMng.h"
#include "../MapCtl.h"
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"

using Vector2Vec = std::vector<Vector2>;


struct CheckView
{
	bool operator()(Vector2 pos, bool flag)
	{
		auto gpID = lpMapCtl.GetMapData(ChipGP::BG, pos, false);					// �����Ɠ��������̃`�b�v
		if (gpID == ChipID::BAR || gpID == ChipID::LAD)								// �����Ɠ��������Ƀo�[�A�͂��������݂���B
		{
			return true;
		}
		if (flag)
		{
			if (gpID == ChipID::BBL || gpID == ChipID::BL)
			{
				return false;
			}
		}

		auto btID = lpMapCtl.GetMapData(ChipGP::BG, { pos.x, pos.y + lpSceneMng.chipSize_.y }, false);					// �����̂P�i���̃`�b�v�i�u���b�N�ށj
		if (btID == ChipID::BBL || btID == ChipID::BL || btID == ChipID::TRAP || btID == ChipID::LAD || btID == ChipID::BAR)			//�������猩�ĂP�i���Ɍ@���u���b�N�i�@���Ă��邩�͖����j�A�@��Ȃ��u���b�N�A�g���b�v�A�͂����A�o�[�����݂��Ă���B
		{
			return true;
		}
		auto atemID = lpMapCtl.GetMapData(ChipGP::ITEM, { pos.x, pos.y + lpSceneMng.chipSize_.y }, false);				// �����̂P�i���̃`�b�v�i�A�C�e���ށj
		if (atemID == ChipID::GOLD)																										//�������猩�ĂP�i���ɋ��򂪑��݂��Ă���ꍇ
		{
			return true;
		}

		return false;
	}
};


struct CheckUp
{
	void operator()(Vector2 pos, Vector2Vec& movePoint)
	{
		// �P�i�ڂ��͂����ł��邩�̃`�F�b�N
		if (lpMapCtl.GetMapData(ChipGP::BG, pos, false) != ChipID::LAD)
		{
			// �͂����łȂ������ꍇ�̓`�F�b�N�I��
			return;
		}

		auto drawOffset = lpMapCtl.GetDrawOffset();
		auto chipSize = lpSceneMng.chipSize_;

		// ���E
		auto checkSide = [&](Vector2 pos, Vector2 offset) {
			if (lpMapCtl.GetMapData(ChipGP::BG, pos + offset, false) == ChipID::BAR)
			{
				movePoint.push_back(pos);
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// �~�̒��S�_�iX���W�j
				//			   drawOffset.y + pos.y + chipSize.y / 2,				// �~�̒��S�_�iY���W�j
				//			   6,																// ���a
				//			   0xff0000, true);
			}
		};

		// ���E��
		auto checkSideDown = [&](Vector2 pos, Vector2 offset) {
			auto ckID = lpMapCtl.GetMapData(ChipGP::BG, pos + offset, false);
			if (ckID == ChipID::BBL || ckID == ChipID::BL)
			{
				movePoint.push_back(pos);
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// �~�̒��S�_�iX���W�j
				//			   drawOffset.y + pos.y + chipSize.y / 2,				// �~�̒��S�_�iY���W�j
				//			   6,													// ���a
				//			   0xff0000, true);
			}
		};

		// �X�^�[�g�n�_����͂������P�i�オ�����ʒu���猟���J�n
		for (pos.y -= chipSize.y; pos.y >= 0; pos.y -= chipSize.y)					// for (auto ckPos = Vector2{ pos.x,pos.y - chipSize.y }; pos.y >= 0; pos.y -= chipSize.y)
		{
			// �������͂����ł��邩�̃`�F�b�N
			if (lpMapCtl.GetMapData(ChipGP::BG, pos, false) != ChipID::LAD)
			{
				// �͂�������Ȃ������i����j
				movePoint.push_back(pos);											// �f�[�^�̍Ō�Ƀf�[�^���i�[
				// �Ԃ̓_
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// �~�̒��S�_�iX���W�j
				//			   drawOffset.y + pos.y + chipSize.y / 2,				// �~�̒��S�_�iY���W�j
				//			   6,													// ���a
				//			   0xff0000, true);
				return;
			}
			// �΂̏c��
			_dbgDrawBox(drawOffset.x + pos.x + chipSize.x / 2 - 2,
						drawOffset.y + pos.y,
						drawOffset.x + pos.x + chipSize.x / 2 + 2,
						drawOffset.y + pos.y + chipSize.y,
						0x00ff00, true);

			// ���̃I�u�W�F�N�g�`�F�b�N
			checkSide(pos, Vector2{ -chipSize.x,0 });			// ��
			checkSide(pos, Vector2{ chipSize.x,0 });			// �E

			// �΂߉��̃I�u�W�F�N�g�`�F�b�N
			checkSideDown(pos, Vector2{ -chipSize.x,chipSize.y });			// ����
			checkSideDown(pos, Vector2{ chipSize.x,chipSize.y });			// �E��
		}
	}
};


struct CheckDown
{
	void operator()(Vector2 pos, const Vector2& plPos, Vector2Vec& movePoint)
	{
		auto drawOffset = lpMapCtl.GetDrawOffset();
		auto chipSize = lpSceneMng.chipSize_;
		auto mapEndPos = lpSceneMng.worldArea_ * chipSize;

		auto checkB = [&](const Vector2& pos) {
			// ����
			auto ckPos = pos + Vector2{ -chipSize.x,chipSize.y };
			auto idLD = lpMapCtl.GetMapData(ChipGP::BG, ckPos, false);
			if (idLD == ChipID::BBL || idLD == ChipID::BL || idLD == ChipID::LAD)
			{
				movePoint.push_back(pos);											// �f�[�^���i�[
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// �~�̒��S�_�iX���W�j
				//			   drawOffset.y + pos.y + chipSize.y / 2,				// �~�̒��S�_�iY���W�j
				//			   6,																// ���a
				//			   0xff0000, true);
				return true;
			}
			// �E��
			ckPos = pos + Vector2{ chipSize.x,chipSize.y };
			auto idRD = lpMapCtl.GetMapData(ChipGP::BG, ckPos, false);
			if (idRD == ChipID::BBL || idRD == ChipID::BL || idRD == ChipID::LAD || idRD == ChipID::BAR)
			{
				movePoint.push_back(pos);				// �f�[�^���i�[
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// �~�̒��S�_�iX���W�j
				//			   drawOffset.y + pos.y + chipSize.y / 2,				// �~�̒��S�_�iY���W�j
				//			   6,																// ���a
				//			   0xff0000, true);
				return true;
			}
			// ��
			ckPos = pos + Vector2{ -chipSize.x,0 };
			auto idL = lpMapCtl.GetMapData(ChipGP::BG, ckPos, false);
			if (idL == ChipID::BAR)
			{
				movePoint.push_back(pos);				// �f�[�^���i�[
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// �~�̒��S�_�iX���W�j
				//	drawOffset.y + pos.y + chipSize.y / 2,				// �~�̒��S�_�iY���W�j
				//	6,																// ���a
				//	0xff0000, true);
				return true;
			}
			return false;
		};

		// �`�F�b�N�J�n�ʒu�̉����u���b�N�̏ꍇ�͂͂���
		auto idD = lpMapCtl.GetMapData(ChipGP::BG, pos + Vector2{ 0,chipSize.y }, false);
		if (idD == ChipID::BBL || idD == ChipID::BL || idD == ChipID::TRAP)
		{
			return;
		}

		for (; pos.y < mapEndPos.y; pos.y += chipSize.y)
		{
			auto id = lpMapCtl.GetMapData(ChipGP::BG, pos, true);
			if (id == ChipID::BBL || id == ChipID::BL)
			{
				// �N�_�Ƀu���b�N�����݂��Ă���ꍇ�͏������Ȃ�
				// �@���Ă���u���b�N�͒ʉ߂�������
				return;
			}
			_dbgDrawBox(drawOffset.x + pos.x + chipSize.x / 2 - 2,
						drawOffset.y + pos.y,
						drawOffset.x + pos.x + chipSize.x / 2 + 2,
						drawOffset.y + pos.y + chipSize.y,
						0x00ff00, true);
			// �v���C���[���Ⴂ�ʒu���̃`�F�b�N
			if (pos.y < plPos.y)
			{
				continue;
			}
			// ����A�̃`�F�b�N������
			id = lpMapCtl.GetMapData(ChipGP::BG, pos, false);
			auto atemID = lpMapCtl.GetMapData(ChipGP::ITEM, pos, false);
			if (atemID == ChipID::GOLD || id == ChipID::BBL || id == ChipID::LAD || id == ChipID::BAR || id == ChipID::TRAP)
			{
				if (checkB(pos))
				{
					continue;
				}
			}
			// ��ɂ��ǂ蒅���Ă��邩�̃`�F�b�N
			id = lpMapCtl.GetMapData(ChipGP::BG, pos + Vector2{ 0,chipSize.y }, false);
			if (id == ChipID::BBL || id == ChipID::BL)
			{
				// �N�_�Ƀu���b�N�����݂��Ă���ꍇ�͏������Ȃ�
				// �@���Ă���u���b�N�͒ʉ߂�������
				movePoint.push_back(pos);
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// �~�̒��S�_�iX���W�j
				//			   drawOffset.y + pos.y + chipSize.y / 2,				// �~�̒��S�_�iY���W�j
				//			   6,																// ���a
				//			   0xff0000, true);
				return;
			}
		}
	}
};


struct SearchModeA
{
	DIR_LR operator()(const Vector2& enPos, const Vector2& plPos)
	{
		auto chipSize = lpSceneMng.chipSize_;

		if (enPos.y != plPos.y)			// Y���W���Ⴄ�ꍇ�͏������Ȃ�
		{
			return DIR_LR::NON;
		}

		// �G�ƃv���C���[�̍��W����͐����̂Ă��Y��ȍ��W�ɂ���(�}�X�ڂ҂�����̍��W�ɂ���)
		auto enPosCor = (enPos / chipSize.x) * chipSize.x;
		auto plPosCor = (plPos / chipSize.x) * chipSize.x;

		int num = (enPosCor.x - plPosCor.x > 0 ? -chipSize.x : chipSize.x);
		for (auto pos = enPosCor; pos.x != plPosCor.x; pos.x += num)
		{
			if (!CheckView()(pos, false))
			{
				return DIR_LR::NON;
			}
		}

		if (enPos.x > plPos.x)			// �v���C���[�����ɂ���
		{
			return DIR_LR::LEFT;
		}
		if (enPos.x < plPos.x)
		{
			return DIR_LR::RIGHT;		// �v���C���[���E�ɂ���
		}
		return DIR_LR::NON;				// �v���C���[��X���W������
	}
};


struct SearchModeB
{
	Vector2Vec operator()(const Vector2& enPos, const Vector2& plPos)
	{
		Vector2Vec movePoint;

		auto drawOffset = lpMapCtl.GetDrawOffset();
		auto chipSize = lpSceneMng.chipSize_;
		auto mapEndPos = lpSceneMng.worldArea_ * chipSize;

		// �G�ƃv���C���[�̍��W����͐����̂Ă��Y��ȍ��W�ɂ���(�}�X�ڂ҂�����̍��W�ɂ���)
		auto enPosCor = (enPos / chipSize.x) * chipSize.x;
		auto plPosCor = (plPos / chipSize.x) * chipSize.x;
		
		//// �����̃`�F�b�N
		//for (auto pos = enPosCor; pos.x >= 0; pos.x -= chipSize.x)
		//{
		//	CheckDown()(pos, plPos, movePoint);
		//	if (!CheckView()(pos, true))
		//	{
		//		break;
		//	}
		//	_dbgDrawBox(drawOffset.x + pos.x, 
		//				drawOffset.y + pos.y + chipSize.y / 2 - 2, 
		//				drawOffset.x + pos.x + chipSize.x, 
		//				drawOffset.y + pos.y + chipSize.y / 2 + 2, 
		//				0x0000ff, true);
		//	CheckUp()(pos, movePoint);
		//}
		//// �E���̃`�F�b�N
		//for (auto pos = Vector2{ enPosCor.x + chipSize.x,enPosCor.y }; pos.x < mapEndPos.x; pos.x += chipSize.x)
		//{
		//	CheckDown()(pos, plPos, movePoint);
		//	if (!CheckView()(pos, true))
		//	{
		//		break;
		//	}
		//	_dbgDrawBox(drawOffset.x + pos.x,
		//				drawOffset.y + pos.y + chipSize.y / 2 - 2,
		//				drawOffset.x + pos.x + chipSize.x,
		//				drawOffset.y + pos.y + chipSize.y / 2 + 2,
		//				0x0000ff, true);
		//	CheckUp()(pos, movePoint);
		//}

		auto checkUD = [&](const Vector2& pos) {
			CheckDown()(pos, plPos, movePoint);
			if (!CheckView()(pos, true))
			{
				return false;
			}
			// �n�����������ꍇ�A�㉺�̎�����`�F�b�N����
			_dbgDrawBox(drawOffset.x + pos.x,
				drawOffset.y + pos.y + chipSize.y / 2 - 2,
				drawOffset.x + pos.x + chipSize.x,
				drawOffset.y + pos.y + chipSize.y / 2 + 2,
				0x0000ff, true);

			CheckUp()(pos, movePoint);
			return true;
		};

		// �v���C���[�̈ʒu�܂Œn���������`�F�b�N����
		for (auto pos = enPos; pos.x < mapEndPos.x; pos.x += chipSize.x)
		{
			if (!checkUD(pos))
			{
				break;
			}
		}
		for (auto pos = enPos - Vector2{ chipSize.x,0 }; pos.x >= 0; pos.x -= chipSize.x)
		{
			if (!checkUD(pos))
			{
				break;
			}
		}

		// ���ɋ߂�����I�����A���E���������ɂ���ꍇ�͍���D��
		std::sort(movePoint.begin(), movePoint.end(), [&](Vector2& p1, Vector2& p2) {
			return (p1.x < p2.x);
			});																							// �ň��Ȃ��Ƃ��悢�i���x����ړI�j
		std::sort(movePoint.begin(), movePoint.end(), [&](Vector2& p1, Vector2& p2) {
			return (abs(enPos.x - p1.x) < abs(enPos.x - p2.x));
			});

		return movePoint;
	}
};

struct SearchModeC
{
	Vector2 operator()(Vector2Vec& movePoint, const Vector2& plPos)
	{
		if (movePoint.size() == 0)
		{
			return Vector2{ 0,0 };
		}

		// �T�[�`���[�hC�̗D��x�ɏ]�����\�[�g���s��
		std::sort(movePoint.begin(), movePoint.end(), [&](const Vector2& p1, const Vector2& p2) {
			int p1Y = p1.y - plPos.y;
			int p2Y = p2.y - plPos.y;
			if (p1Y > 0 && p2Y > 0)
			{
				return p1Y < p2Y;
			}
			return p1Y > p2Y;
			});

		return *movePoint.begin();
	}
};
