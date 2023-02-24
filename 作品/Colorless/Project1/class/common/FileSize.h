#pragma once
#include "Vector2.h"
//--------------------------------------------------------------------------
// FileSize：画像ファイル等のサイズ等を格納
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

	//【画像ファイル】
	const Vector2 blockImageSize_{ 288,32 };//ブロック全体
	const Vector2 blockSize_{ 32,32 };//ブロック単体
	const Vector2 blockDiv_{ 9, 1 };//ブロック分割数
	const Vector2 cursorSize_{ 48,48 };//カーソル

	const Vector2 titleLogoSize_{ 700,175 };//タイトルシーンタイトルロゴ
	const Vector2 titleKeySize_{ 359,103 };//タイトルシーンキーロゴ

	const Vector2 selectLogoSize_{ 600,65 };//セレクトシーンセレクトロゴ

	const Vector2 callImageSize_{ 300,240 };//カウントダウン全体
	const Vector2 callDiv_{ 1,3 };//カウントダウン分割数
	const Vector2 ruleSize_{ 600,217 };//ルール

	const Vector2 menuImageSize_{ 350,240 };//メニューロゴ全体
	const Vector2 menuSize_{ 350,60 };//メニューロゴ単体
	const Vector2 menuDiv_{ 1,4 };//メニューロゴ分割数

	const Vector2 clearLogoSize_{ 500,80 };//クリアシーンクリアロゴ

private:
	FileSize() {};
	~FileSize() {};
};
