#include <DxLib.h>
#include "Mouse.h"
#include "../../_debug/_DebugConOut.h"


Mouse::Mouse()
{
	//TRACE("Mouseを生成\n");
	Init();
}


Mouse::Mouse(Vector2 offset)
{
	//TRACE("Mouseを生成\n");
	Init();
	offset_ = offset;
}


Mouse::~Mouse()
{
}


bool Mouse::Update(Obj& obj)
{
	GetMousePoint(&data_["X座標"], &data_["Y座標"]);
	data_["X座標"] -= offset_.x;				// ゲーム中のオフセット分ずらす
	data_["Y座標"] -= offset_.y;				// ゲーム中のオフセット分ずらす
	data_["ボタン"] = GetMouseInput() & MOUSE_INPUT_LEFT;
	data_["ホイール"] = GetMouseWheelRotVol();
	data_["リセット"] = CheckHitKey(KEY_INPUT_F9);
	data_["セーブ"] = CheckHitKey(KEY_INPUT_F8);
	data_["ロード"] = CheckHitKey(KEY_INPUT_F7);
	data_["プレイ"] = CheckHitKey(KEY_INPUT_F5);
	data_["ヘルプ"] = CheckHitKey(KEY_INPUT_F4);
	data_["ブロック一覧"] = CheckHitKey(KEY_INPUT_F3);

	return true;
}


bool Mouse::Init()
{
	data_["X座標"] = 0;
	data_["Y座標"] = 0;
	data_["ボタン"] = 0;
	data_["ホイール"] = 0;
	data_["リセット"] = 0;
	data_["セーブ"] = 0;
	data_["ロード"] = 0;
	data_["プレイ"] = 0;
	data_["ヘルプ"] = 0;
	data_["ブロック一覧"] = 0;

	return true;
}
