#pragma once
#include <map>		// 連想配列
#include <vector>	// メモリの連続性、非連続性
#include <string>
#include "Vector2.h"

#define lpImageMng ImageMng::GetInstance()
using VecInt = std::vector<int>;

class ImageMng
{
public:
	static ImageMng& GetInstance(void)
	{
		static ImageMng s_Instance;
		return s_Instance;
	}
	const VecInt& GetID(std::string key);	// 表示用
	const VecInt& GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt);	// ロード用

private:
	ImageMng();
	~ImageMng();

	std::map<std::string, VecInt> imgMap_;			// イメージ情報
};
