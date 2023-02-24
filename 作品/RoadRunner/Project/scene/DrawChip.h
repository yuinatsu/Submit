#pragma once
#include <DxLib.h>
#include "MapCtl.h"
#include "../common/ImageMng.h"
#include "../common/Vector2.h"


// 標準描画関数
//---------------
struct DrawChipDef
{
	void operator()(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)			// drawOffsetはマス目数
	{
		DrawGraph(x * chipSize.x + drawOffset.x, y * chipSize.y + drawOffset.y, imageID, true);
	}
};


// 何も描画しない描画関数
//-------------------------
struct DrawChipNon
{
	void operator()(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)
	{
		// 何も描画しない
	}
};


// 脱出はしご描画関数		金塊を集めきったら普通のはしごとして描画
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


// トラップ描画関数			見た目は掘れるブロックにする
//-------------------
struct DrawChipTrap
{
	void operator()(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)
	{
		DrawGraph(x * chipSize.x + drawOffset.x, y * chipSize.y + drawOffset.y, lpImageMng.GetID("tileset")[static_cast<int>(ChipID::BBL)], true);
	}
};


// 壊せるブロック描画関数
//-------------------------
//#define DigCnt 10														// １コマ当たりのフレーム数
//#define DigFrameLim 5													// コマ数
//#define DigWait (60 * 3)												// 3秒間の待機時間
//#define DigFullFrame ((DigCnt * DigFrameLim) * 2 + DigWait)			// 総フレーム数
struct DrawChipBbl
{
	void operator()(const int x, const int y, const Vector2& chipSize, const Vector2& drawOffset, const int imageID)
	{
		// 穴を掘る際の砂ぼこりエフェクト実装予定
		int id = 0;
		int count = lpMapCtl.GetDigData(x, y);
		if ((count / DigCnt) == 0)
		{
			id = imageID;
		}
		else
		{
			if (count > ((DigCnt * DigFrameLim) + DigWait))					// 穴掘りパート						// DigFullFrame ... (DigCnt * DigFrameLim) -> DigWait -> (DigCnt * DigFrameLim) ...
			{
				int point = ((DigFullFrame - count) / DigCnt) * static_cast<int>(ChipID::MAX);										// (DigFullFrame - count) ... 初期値０からカウントアップさせるためにする			DigCnt * static_cast<int>(ChipID::MAX) ... １０刻みじゃなく１１刻みさせる
				id = lpImageMng.GetID("tileset")[static_cast<int>(ChipID::BBL) + point];						// アニメーションが一つ進むごとに11加算される
			}
			if (count <= (DigCnt * DigFrameLim))							// 穴修復パート
			{
				int point = (DigFrameLim - ((DigCnt * DigFrameLim) - count) / DigCnt) * static_cast<int>(ChipID::MAX) + 1;			// DigFrameLimから引くとMAXから０に近づいていく		((DigCnt * DigFrameLim) - count) ... 初期値０からカウントアップさせるためにする
				id = lpImageMng.GetID("tileset")[static_cast<int>(ChipID::BBL) + point];
			}
		}
		if (id)
		{
			DrawGraph(x * chipSize.x + drawOffset.x, y * chipSize.y + drawOffset.y, id, true);
		}
	}
};
