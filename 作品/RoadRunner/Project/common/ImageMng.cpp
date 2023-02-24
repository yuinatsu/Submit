#include <DxLib.h>
#include "ImageMng.h"
#include "../_debug/_DebugConOut.h"


const VecInt& ImageMng::GetID(std::string key)
{
	// ガード処理
	if (ImageMap_.find(key) == ImageMap_.end())					// keyの値が存在しなかったら中に入る
	{
		ImageMap_[key].resize(1);								// 要素数１の画像として読み込む
		ImageMap_[key][0] = LoadGraph(key.c_str());				// 画像の読み込み
	}
	return ImageMap_[key];										// 画像をリターン
}


const VecInt& ImageMng::GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt)
{
	if (ImageMap_.find(key) == ImageMap_.end())																							// 画像が読み込んでいなかったら中に入る		(ImageMap_.end ... ImageMap.findの結果最初から最後までどこにもなかった場合真になる)			//ImageMap_.at(f_name);
	{
		//TRACE("ファイルの読み込み\n");
		ImageMap_[key].resize(divCnt.x * divCnt.y);																						// 画像のファイルネーム読み込み
		LoadDivGraph(f_name.c_str(), divCnt.x * divCnt.y, divCnt.x, divCnt.y, divSize.x, divSize.y, &ImageMap_[key][0], true);			// 画像の読み込み	c_str ... char型をstring型に変える
	}
	return ImageMap_[key];																												// 画像をリターン
}


ImageMng::ImageMng()
{
}


ImageMng::~ImageMng()
{
}
