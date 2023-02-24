#pragma once
#include "common/Vector2.h"
//--------------------------------------------------------------------------
// FileSize：画像ファイル等のサイズを格納するヘッダです。
//--------------------------------------------------------------------------

#define lpSize FileSize::GetInstance()

class FileSize
{
public:
	static FileSize& GetInstance(void)
	{
		static FileSize s_Instance;
		return s_Instance;
	}

	//const Vector2 scrSize_{ 720,480 };//画面解像度//SceneCtlのscrArea_と同義
	const Vector2 titleLogo_{ 400,90 };//タイトルロゴ
	const Vector2 titleSect1_{ 300,35 };//タイトル項目１
	const Vector2 titleSect2_{ 280,35 };//タイトル項目２
	const Vector2 titleSect3_{ 270,35 };//タイトル項目３
	
	const Vector2 dataSect1_{ 300,35 };//データベース項目１（Weapon）
	const Vector2 dataSect2_{ 290,35 };//データベース項目２（Perform）

	const Vector2 mimiIse_{ 171,300 };//耳（伊勢）
	const Vector2 mimiImiya_{ 202,300 };//耳（井宮）
	const Vector2 mimiUrayama_{ 178,300 };//耳（浦山）
	const Vector2 mimiKakita_{ 166,300 };//耳（柿田）
	const Vector2 mimiKaziwara_{ 186,300 };//耳（梶原）

	const Vector2 mimiGraph_{ 171,300 };//耳グラフィック

	const Vector2 wepIcon_{ 94,97 };//武器アイコン
	const Vector2 wepFlame_{ 100,120 };//武器フレーム

	const Vector2 wepData_{ 430,286 };// 武器説明
	const Vector2 perItem_{ 160,80 };// 実績項目
	const Vector2 perData_{ 230,32 };// 実績テキスト

	const Vector2 numFont_{ 60,60 };//数字フォント

private:
	FileSize() {};
	~FileSize() {};
};
