#include <algorithm>
#include <memory>
#include <vector>
#include <chrono>
#include <dxlib.h>
#include "Graph/Square.h"
#include "Graph/Circle.h"
#include "Graph/Triangle.h"
#include "Graph/Star.h"
#include "Graph/Tree.h"
#include "Graph/Micky.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"


bool SysInit(void)
{
	SetWindowText("GraphMove");
	SetGraphMode(1024, 768, 32);// (X座標, Y座標, 解像度)
	ChangeWindowMode(true);// (true : Window, false : フルスクリーン)
	if (DxLib_Init() == -1)
	{
		return false;// 異常終了
	}
	return true;// 正常終了
}


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	if (!SysInit())
	{
		return -1;// 異常終了
	}

	srand((unsigned int)time(NULL));

	// shapeを追加
	ShapeVec shapeVec;
	shapeVec.reserve(10);
	shapeVec.emplace_back(std::make_unique<Square>(Vector2Flt{ 100, 100 }, Vector2Flt{ 200, 200 }));
	shapeVec.emplace_back(std::make_unique<Circle>(Vector2Flt{ 300, 100 }, Vector2Flt{ 50, 50 }));
	shapeVec.emplace_back(std::make_unique<Triangle>(Vector2Flt{ 100, 350 }, Vector2Flt{ 50, 50 }));
	shapeVec.emplace_back(std::make_unique<Star>(Vector2Flt{ 400, 200 }, Vector2Flt{ 50, 50 }));
	shapeVec.emplace_back(std::make_unique<Tree>(Vector2Flt{ 300, 300 }, Vector2Flt{ 50, 50 }));
	shapeVec.emplace_back(std::make_unique<Micky>(Vector2Flt{ 200, 200 }, Vector2Flt{ 30, 30 }));

	// deltaTime定義
	std::chrono::system_clock::time_point nowTime, oldTime;
	nowTime = std::chrono::system_clock::now();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)// windowを閉じる, ESCキーの入力
	{
		_dbgStartDraw();

		// deltaTime計算
		oldTime = nowTime;
		nowTime = std::chrono::system_clock::now();
		auto elTime = nowTime - oldTime;
		auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(elTime).count();
		auto delta = msec / 1000.0f;

		// 各shapeを描画
		for (auto& shape : shapeVec)
		{
			shape->Update(delta, shapeVec);
		}

		ClsDrawScreen();
		// 各shapeを描画
		for (auto& shape : shapeVec)
		{
			shape->Draw();
		}
		ScreenFlip();
	}
	return 0;// 正常終了
}
