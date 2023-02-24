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
	//for (auto& data : data_)			// 入力情報がNOWかOLDかの情報取得
	//{
	//	dataOld_[data.first] = data.second;
	//}
	//if ((obj.GetPos() % lpSceneMng.chipSize_) == Vector2{ 0,0 })
	//{
	//	for (auto& data : data_)			// 0クリアは升目ぴったり飲み
	//	{
	//		data.second = 0;
	//	}
	//}
	//else
	//{
	//	return false;
	//}
	//_dbgStartDraw();				// デバッグ表示のバグを防ぐためここに設置
	//auto moveDir = SearchModeA()(obj.GetPos(), other.GetPos());
	//if (moveDir != DIR_LR::NON)
	//{
	//	/*条件に見合ったキーに１をセットする。*/
	//	if (moveDir == DIR_LR::LEFT)
	//	{
	//		data_["左"] = 1;
	//	}
	//	if (moveDir == DIR_LR::RIGHT)
	//	{
	//		data_["右"] = 1;
	//	}
	//}
	//else
	//{
	//	auto drawOffset = lpMapCtl.GetDrawOffset();
	//	auto chipSize = lpSceneMng.chipSize_;
	//	auto otherPos = other.GetPos();
	//	auto enPos = obj.GetPos();
	//	auto setMovePoint = [&](const Vector2& point) {
	//		_dbgDrawCircle(drawOffset.x + point.x + chipSize.x / 2,				// 円の中心点（X座標）
	//					   drawOffset.y + point.y + chipSize.y / 2,				// 円の中心点（Y座標）
	//					   3,													// 半径
	//					   0x00ff00, true);
	//		if (enPos.x > point.x)
	//		{
	//			data_["左"] = 1;
	//		}
	//		else if (enPos.x < point.x)
	//		{
	//			data_["右"] = 1;
	//		}
	//		else
	//		{
	//			if (enPos.y > point.y)
	//			{
	//				data_["上"] = 1;
	//			}
	//			else if (enPos.y < point.y)
	//			{
	//				data_["下"] = 1;
	//			}
	//			else
	//			{
	//				data_["下"] = 1;					// 目的地と同じ位置にキャラがいる場合
	//			}
	//		}
	//	};
	//	auto movePoint = std::move(SearchModeB()(obj.GetPos() + Vector2{ 0,chipSize.y - 1 }, otherPos));
	//	for (const auto& point : movePoint)
	//	{
	//		if (point.y == otherPos.y)
	//		{
	//			setMovePoint(point);
	//			//_dbgDrawCircle(drawOffset.x + point.x + chipSize.x / 2,				// 円の中心点（X座標）
	//			//			   drawOffset.y + point.y + chipSize.y / 2,				// 円の中心点（Y座標）
	//			//			   3,																// 半径
	//			//			   0x00ff00, true);
	//			return true;
	//		}
	//	}
	//	// サーチモードC
	//	setMovePoint(SearchModeC()(movePoint, enPos));
	//}
	////data_["上"] = keyData_[keyTable_["上"]];
	////data_["下"] = keyData_[keyTable_["下"]];
	////data_["左"] = keyData_[keyTable_["左"]];
	////data_["右"] = keyData_[keyTable_["右"]];
	////data_["Ａ"] = keyData_[keyTable_["Ａ"]];
	////data_["エディット"] = keyData_[keyTable_["エディット"]];


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
			data_["左"] = 1;
		}
		if (dir == DIR_LR::RIGHT)
		{
			data_["右"] = 1;
		}
		_dbgMes = "サーチモードA";
	}
	else
	{
		auto enPos = obj.GetPos();

		auto SetMovePoint = [&](const Vector2& point) {
			if (enPos.x > point.x)
			{
				data_["左"] = 1;
			}
			else if (enPos.x < point.x)
			{
				data_["右"] = 1;
			}
			else
			{
				if (enPos.y > point.y)
				{
					data_["上"] = 1;
				}
				else if (enPos.y < point.y)
				{
					data_["下"] = 1;
				}
				else
				{
					data_["下"] = 1;					// 目的地と同じ位置にキャラがいる場合
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
				_dbgMes = "サーチモードB";
				SetMovePoint(point);
				//_dbgDrawCircle(drawOffset.x + point.x + chipSize.x / 2,				// 円の中心点（X座標）
				//	drawOffset.y + point.y + chipSize.y / 2,				// 円の中心点（Y座標）
				//	3,													// 半径
				//	0x00ff00, true);
				keySetEX();
				return true;
			}
		}
		_dbgMes = "サーチモードC";
		auto point = SearchModeC()(pointData, plPos);
		//_dbgDrawCircle(drawOffset.x + point.x + chipSize.x / 2,				// 円の中心点（X座標）
		//	drawOffset.y + point.y + chipSize.y / 2,				// 円の中心点（Y座標）
		//	3,													// 半径
		//	0x00ff00, true);
		SetMovePoint(point);
	}
	keySetEX();

	return true;
}
