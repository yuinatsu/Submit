#pragma once
#include <map>
#include <vector>
#include <string>
#include "Vector2.h"

#define lpImageMng ImageMng::GetInstance()

using VecInt = std::vector<int>;					// int型の配列

class ImageMng
{
public:
	static ImageMng& GetInstance()
	{
		static ImageMng s_Instance;				// s_Instance	シングルトンインスタンス
		return s_Instance;
	}
	const VecInt& GetID(std::string key);																	// GetID(キー値)			存在しないキー値が来た場合に使用
	const VecInt& GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt);				// GetID(ファイルネーム, キー値, 分割総数, 分割数)		今回はわかりやすさ重視でstringを使用
private:
	ImageMng();
	~ImageMng();
	std::map<std::string, VecInt> ImageMap_;						// 
};

