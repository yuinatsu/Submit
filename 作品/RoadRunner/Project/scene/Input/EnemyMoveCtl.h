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
		auto gpID = lpMapCtl.GetMapData(ChipGP::BG, pos, false);					// 自分と同じ高さのチップ
		if (gpID == ChipID::BAR || gpID == ChipID::LAD)								// 自分と同じ高さにバー、はしごが存在する。
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

		auto btID = lpMapCtl.GetMapData(ChipGP::BG, { pos.x, pos.y + lpSceneMng.chipSize_.y }, false);					// 自分の１段下のチップ（ブロック類）
		if (btID == ChipID::BBL || btID == ChipID::BL || btID == ChipID::TRAP || btID == ChipID::LAD || btID == ChipID::BAR)			//自分から見て１段下に掘れるブロック（掘っているかは無視）、掘れないブロック、トラップ、はしご、バーが存在している。
		{
			return true;
		}
		auto atemID = lpMapCtl.GetMapData(ChipGP::ITEM, { pos.x, pos.y + lpSceneMng.chipSize_.y }, false);				// 自分の１段下のチップ（アイテム類）
		if (atemID == ChipID::GOLD)																										//自分から見て１段下に金塊が存在している場合
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
		// １段目がはしごであるかのチェック
		if (lpMapCtl.GetMapData(ChipGP::BG, pos, false) != ChipID::LAD)
		{
			// はしごでなかった場合はチェック終了
			return;
		}

		auto drawOffset = lpMapCtl.GetDrawOffset();
		auto chipSize = lpSceneMng.chipSize_;

		// 左右
		auto checkSide = [&](Vector2 pos, Vector2 offset) {
			if (lpMapCtl.GetMapData(ChipGP::BG, pos + offset, false) == ChipID::BAR)
			{
				movePoint.push_back(pos);
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// 円の中心点（X座標）
				//			   drawOffset.y + pos.y + chipSize.y / 2,				// 円の中心点（Y座標）
				//			   6,																// 半径
				//			   0xff0000, true);
			}
		};

		// 左右下
		auto checkSideDown = [&](Vector2 pos, Vector2 offset) {
			auto ckID = lpMapCtl.GetMapData(ChipGP::BG, pos + offset, false);
			if (ckID == ChipID::BBL || ckID == ChipID::BL)
			{
				movePoint.push_back(pos);
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// 円の中心点（X座標）
				//			   drawOffset.y + pos.y + chipSize.y / 2,				// 円の中心点（Y座標）
				//			   6,													// 半径
				//			   0xff0000, true);
			}
		};

		// スタート地点からはしごを１段上がった位置から検索開始
		for (pos.y -= chipSize.y; pos.y >= 0; pos.y -= chipSize.y)					// for (auto ckPos = Vector2{ pos.x,pos.y - chipSize.y }; pos.y >= 0; pos.y -= chipSize.y)
		{
			// そこがはしごであるかのチェック
			if (lpMapCtl.GetMapData(ChipGP::BG, pos, false) != ChipID::LAD)
			{
				// はしごじゃなかった（頂上）
				movePoint.push_back(pos);											// データの最後にデータを格納
				// 赤の点
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// 円の中心点（X座標）
				//			   drawOffset.y + pos.y + chipSize.y / 2,				// 円の中心点（Y座標）
				//			   6,													// 半径
				//			   0xff0000, true);
				return;
			}
			// 緑の縦線
			_dbgDrawBox(drawOffset.x + pos.x + chipSize.x / 2 - 2,
						drawOffset.y + pos.y,
						drawOffset.x + pos.x + chipSize.x / 2 + 2,
						drawOffset.y + pos.y + chipSize.y,
						0x00ff00, true);

			// 横のオブジェクトチェック
			checkSide(pos, Vector2{ -chipSize.x,0 });			// 左
			checkSide(pos, Vector2{ chipSize.x,0 });			// 右

			// 斜め下のオブジェクトチェック
			checkSideDown(pos, Vector2{ -chipSize.x,chipSize.y });			// 左下
			checkSideDown(pos, Vector2{ chipSize.x,chipSize.y });			// 右下
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
			// 左下
			auto ckPos = pos + Vector2{ -chipSize.x,chipSize.y };
			auto idLD = lpMapCtl.GetMapData(ChipGP::BG, ckPos, false);
			if (idLD == ChipID::BBL || idLD == ChipID::BL || idLD == ChipID::LAD)
			{
				movePoint.push_back(pos);											// データを格納
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// 円の中心点（X座標）
				//			   drawOffset.y + pos.y + chipSize.y / 2,				// 円の中心点（Y座標）
				//			   6,																// 半径
				//			   0xff0000, true);
				return true;
			}
			// 右下
			ckPos = pos + Vector2{ chipSize.x,chipSize.y };
			auto idRD = lpMapCtl.GetMapData(ChipGP::BG, ckPos, false);
			if (idRD == ChipID::BBL || idRD == ChipID::BL || idRD == ChipID::LAD || idRD == ChipID::BAR)
			{
				movePoint.push_back(pos);				// データを格納
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// 円の中心点（X座標）
				//			   drawOffset.y + pos.y + chipSize.y / 2,				// 円の中心点（Y座標）
				//			   6,																// 半径
				//			   0xff0000, true);
				return true;
			}
			// 左
			ckPos = pos + Vector2{ -chipSize.x,0 };
			auto idL = lpMapCtl.GetMapData(ChipGP::BG, ckPos, false);
			if (idL == ChipID::BAR)
			{
				movePoint.push_back(pos);				// データを格納
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// 円の中心点（X座標）
				//	drawOffset.y + pos.y + chipSize.y / 2,				// 円の中心点（Y座標）
				//	6,																// 半径
				//	0xff0000, true);
				return true;
			}
			return false;
		};

		// チェック開始位置の下がブロックの場合ははじく
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
				// 起点にブロックが存在している場合は処理しない
				// 掘っているブロックは通過させたい
				return;
			}
			_dbgDrawBox(drawOffset.x + pos.x + chipSize.x / 2 - 2,
						drawOffset.y + pos.y,
						drawOffset.x + pos.x + chipSize.x / 2 + 2,
						drawOffset.y + pos.y + chipSize.y,
						0x00ff00, true);
			// プレイヤーより低い位置かのチェック
			if (pos.y < plPos.y)
			{
				continue;
			}
			// 条件Aのチェックをする
			id = lpMapCtl.GetMapData(ChipGP::BG, pos, false);
			auto atemID = lpMapCtl.GetMapData(ChipGP::ITEM, pos, false);
			if (atemID == ChipID::GOLD || id == ChipID::BBL || id == ChipID::LAD || id == ChipID::BAR || id == ChipID::TRAP)
			{
				if (checkB(pos))
				{
					continue;
				}
			}
			// 底にたどり着いているかのチェック
			id = lpMapCtl.GetMapData(ChipGP::BG, pos + Vector2{ 0,chipSize.y }, false);
			if (id == ChipID::BBL || id == ChipID::BL)
			{
				// 起点にブロックが存在している場合は処理しない
				// 掘っているブロックは通過させたい
				movePoint.push_back(pos);
				//_dbgDrawCircle(drawOffset.x + pos.x + chipSize.x / 2,				// 円の中心点（X座標）
				//			   drawOffset.y + pos.y + chipSize.y / 2,				// 円の中心点（Y座標）
				//			   6,																// 半径
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

		if (enPos.y != plPos.y)			// Y座標が違う場合は処理しない
		{
			return DIR_LR::NON;
		}

		// 敵とプレイヤーの座標からは数を捨てて綺麗な座標にする(マス目ぴったりの座標にする)
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

		if (enPos.x > plPos.x)			// プレイヤーが左にいる
		{
			return DIR_LR::LEFT;
		}
		if (enPos.x < plPos.x)
		{
			return DIR_LR::RIGHT;		// プレイヤーが右にいる
		}
		return DIR_LR::NON;				// プレイヤーとX座標が同じ
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

		// 敵とプレイヤーの座標からは数を捨てて綺麗な座標にする(マス目ぴったりの座標にする)
		auto enPosCor = (enPos / chipSize.x) * chipSize.x;
		auto plPosCor = (plPos / chipSize.x) * chipSize.x;
		
		//// 左側のチェック
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
		//// 右側のチェック
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
			// 地続きだった場合、上下の視野をチェックする
			_dbgDrawBox(drawOffset.x + pos.x,
				drawOffset.y + pos.y + chipSize.y / 2 - 2,
				drawOffset.x + pos.x + chipSize.x,
				drawOffset.y + pos.y + chipSize.y / 2 + 2,
				0x0000ff, true);

			CheckUp()(pos, movePoint);
			return true;
		};

		// プレイヤーの位置まで地続きかをチェックする
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

		// 次に近い方を選択し、左右同じ距離にある場合は左を優先
		std::sort(movePoint.begin(), movePoint.end(), [&](Vector2& p1, Vector2& p2) {
			return (p1.x < p2.x);
			});																							// 最悪なくともよい（精度向上目的）
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

		// サーチモードCの優先度に従ったソートを行う
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
