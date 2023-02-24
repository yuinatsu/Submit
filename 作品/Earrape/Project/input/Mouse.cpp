#include <DxLib.h>
#include "Mouse.h"
#include "../_debug/_DebugConOut.h"

Mouse::Mouse()
{
	Init();			// 初期化
}

Mouse::Mouse(Vector2 offset)
{
	Init();			// 初期化
	offset_ = offset;
}

Mouse::~Mouse()
{
}

int Mouse::GetPresCnt(int keyCode)
{
	// 有効なキーじゃなければ抜ける
	if (!isValidKeycode(keyCode))
	{
		return -1;
	}
	// 押している間のフレーム数
	return btnPresCnt_[keyCode];
}

int Mouse::GetRelCnt(int keyCode)
{
	// 有効なキーじゃなければ抜ける
	if (!isValidKeycode(keyCode))
	{
		return -1;
	}
	// 離している間のフレーム数
	return btnRelCnt_[keyCode];
}

bool Mouse::Update(void)
{
	// マウスポインタの位置を取得する
	GetMousePoint(&data_["X座標"], &data_["Y座標"]);
	// オフセット値加算
	data_["X座標"] -= offset_.x;
	data_["Y座標"] -= offset_.y;

	data_["ボタン"] = GetMouseInput();			// マウスのボタンの押下状態を取得する
	data_["ホイール"] = GetMouseWheelRotVol();	// 垂直マウスホイールの回転量を取得する

	for (int i = 0; i < KEY_NUM; i++)
	{
		if ((GetMouseInput() >> i) & 1)
		{
			//i番のボタンが押されていたら
			if (btnRelCnt_[i] > 0)
			{
				clickkCnt_++;
				//離されカウンタが0より大きければ
				btnRelCnt_[i] = 0;	//0に戻す
			}
			btnPresCnt_[i]++;	//押されカウンタを増やす
		}
		else
		{
			//i番のキーが離されていたら
			if (btnPresCnt_[i] > 0)
			{
				//押されカウンタが0より大きければ
				btnPresCnt_[i] = 0;	//0に戻す
			}
			btnRelCnt_[i]++;	//離されカウンタを増やす
		}
	}
	return true;
}

bool Mouse::Init(void)
{
	data_["X座標"]    = 0;
	data_["Y座標"]    = 0;
	data_["ボタン"]   = 0;
	data_["ホイール"] = 0;
	return true;
}

bool Mouse::isValidKeycode(int keyCode)
{
	if (!(0 <= keyCode && keyCode < KEY_NUM))
	{
		return false;
	}
	return true;
}
