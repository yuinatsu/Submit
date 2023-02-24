#include "EnemyInput.h"
#include "EnemyMoveCtl.h"


EnemyInput::EnemyInput()
{
}


EnemyInput::~EnemyInput()
{
}


bool EnemyInput::Update(Obj& obj, Obj& other)
{
	//for (auto& data : data_)			// ���͏��NOW��OLD���̏��擾
	//{
	//	dataOld_[data.first] = data.second;
	//}
	//if ((obj.GetPos() % lpSceneMng.chipSize_) == Vector2{ 0,0 })
	//{
	//	for (auto& data : data_)			// 0�N���A�͏��ڂ҂��������
	//	{
	//		data.second = 0;
	//	}
	//}
	//else
	//{
	//	return false;
	//}
	//_dbgStartDraw();				// �f�o�b�O�\���̃o�O��h�����߂����ɐݒu
	//auto moveDir = SearchModeA()(obj.GetPos(), other.GetPos());
	//if (moveDir != DIR_LR::NON)
	//{
	//	/*�����Ɍ��������L�[�ɂP���Z�b�g����B*/
	//	if (moveDir == DIR_LR::LEFT)
	//	{
	//		data_["��"] = 1;
	//	}
	//	if (moveDir == DIR_LR::RIGHT)
	//	{
	//		data_["�E"] = 1;
	//	}
	//}
	//else
	//{
	//	auto drawOffset = lpMapCtl.GetDrawOffset();
	//	auto chipSize = lpSceneMng.chipSize_;
	//	auto otherPos = other.GetPos();
	//	auto enPos = obj.GetPos();
	//	auto setMovePoint = [&](const Vector2& point) {
	//		_dbgDrawCircle(drawOffset.x + point.x + chipSize.x / 2,				// �~�̒��S�_�iX���W�j
	//					   drawOffset.y + point.y + chipSize.y / 2,				// �~�̒��S�_�iY���W�j
	//					   3,													// ���a
	//					   0x00ff00, true);
	//		if (enPos.x > point.x)
	//		{
	//			data_["��"] = 1;
	//		}
	//		else if (enPos.x < point.x)
	//		{
	//			data_["�E"] = 1;
	//		}
	//		else
	//		{
	//			if (enPos.y > point.y)
	//			{
	//				data_["��"] = 1;
	//			}
	//			else if (enPos.y < point.y)
	//			{
	//				data_["��"] = 1;
	//			}
	//			else
	//			{
	//				data_["��"] = 1;					// �ړI�n�Ɠ����ʒu�ɃL����������ꍇ
	//			}
	//		}
	//	};
	//	auto movePoint = std::move(SearchModeB()(obj.GetPos() + Vector2{ 0,chipSize.y - 1 }, otherPos));
	//	for (const auto& point : movePoint)
	//	{
	//		if (point.y == otherPos.y)
	//		{
	//			setMovePoint(point);
	//			//_dbgDrawCircle(drawOffset.x + point.x + chipSize.x / 2,				// �~�̒��S�_�iX���W�j
	//			//			   drawOffset.y + point.y + chipSize.y / 2,				// �~�̒��S�_�iY���W�j
	//			//			   3,																// ���a
	//			//			   0x00ff00, true);
	//			return true;
	//		}
	//	}
	//	// �T�[�`���[�hC
	//	setMovePoint(SearchModeC()(movePoint, enPos));
	//}
	////data_["��"] = keyData_[keyTable_["��"]];
	////data_["��"] = keyData_[keyTable_["��"]];
	////data_["��"] = keyData_[keyTable_["��"]];
	////data_["�E"] = keyData_[keyTable_["�E"]];
	////data_["�`"] = keyData_[keyTable_["�`"]];
	////data_["�G�f�B�b�g"] = keyData_[keyTable_["�G�f�B�b�g"]];


	static bool keyboardF = false;
	if (CheckHitKey(KEY_INPUT_F2))
	{
		keyboardF = true;
	}
	if (CheckHitKey(KEY_INPUT_F3))
	{
		keyboardF = false;
	}
	auto keySetEX = [&]() {
		if (keyboardF)
		{
			for (auto& data : data_)
			{
				dataOld_[data.first] = data.second;
			}

			GetHitKeyStateAll(keyData_);
			for (auto tblData : keyTable_)
			{
				data_[tblData.first] = keyData_[tblData.second];
			}
			return false;
		}
		return true;
	};

	static std::string _dbgMes;
	if ((obj.GetPos() % lpSceneMng.chipSize_) == Vector2{ 0,0 })
	{
		_dbgStartDraw();
		//_dbgDrawFormatString(0, 0, 0xffffff, "%s", _dbgMes.c_str());
		for (auto& data: data_)
		{
			dataOld_[data.first] = data.second;
			data.second = 0;
		}
	}
	else
	{
		//_dbgDrawFormatString(0, 0, 0xffffff, "%s", _dbgMes.c_str());
		return false;
	}
	if (obj.GetPos() == other.GetPos())
	{
		return false;
	}
	auto dir = SearchModeA()(obj.GetPos(), other.GetPos());
	if (dir != DIR_LR::NON)
	{
		if (dir == DIR_LR::LEFT)
		{
			data_["��"] = 1;
		}
		if (dir == DIR_LR::RIGHT)
		{
			data_["�E"] = 1;
		}
		_dbgMes = "�T�[�`���[�hA";
	}
	else
	{
		auto enPos = obj.GetPos();

		auto SetMovePoint = [&](const Vector2& point) {
			if (enPos.x > point.x)
			{
				data_["��"] = 1;
			}
			else if (enPos.x < point.x)
			{
				data_["�E"] = 1;
			}
			else
			{
				if (enPos.y > point.y)
				{
					data_["��"] = 1;
				}
				else if (enPos.y < point.y)
				{
					data_["��"] = 1;
				}
				else
				{
					data_["��"] = 1;					// �ړI�n�Ɠ����ʒu�ɃL����������ꍇ
				}
			}
		};
		
		auto plPos = other.GetPos();
		Vector2 offset{ 0,offset.y = lpSceneMng.chipSize_.y - 1 };
		auto pointData = std::move(SearchModeB()(obj.GetPos() + offset, plPos));
		auto chipSize = lpSceneMng.chipSize_;
		auto drawOffset = lpMapCtl.GetDrawOffset();
		for (const auto& point : pointData)
		{
			if (point.y == plPos.y)
			{
				_dbgMes = "�T�[�`���[�hB";
				SetMovePoint(point);
				//_dbgDrawCircle(drawOffset.x + point.x + chipSize.x / 2,				// �~�̒��S�_�iX���W�j
				//	drawOffset.y + point.y + chipSize.y / 2,				// �~�̒��S�_�iY���W�j
				//	3,													// ���a
				//	0x00ff00, true);
				keySetEX();
				return true;
			}
		}
		_dbgMes = "�T�[�`���[�hC";
		auto point = SearchModeC()(pointData, plPos);
		//_dbgDrawCircle(drawOffset.x + point.x + chipSize.x / 2,				// �~�̒��S�_�iX���W�j
		//	drawOffset.y + point.y + chipSize.y / 2,				// �~�̒��S�_�iY���W�j
		//	3,													// ���a
		//	0x00ff00, true);
		SetMovePoint(point);
	}
	keySetEX();

	return true;
}
