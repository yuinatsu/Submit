#pragma once
#include <vector>
#include <string>
#include <map>
#include "Vector2.h"

#define lpImageMng ImageMng::GetInstance()

class ImageMng
{
public:
	static ImageMng& GetInstance() 
	{
		static ImageMng s_Instance;
		return s_Instance;
	}
	const VecInt& GetID(std::string key);													// ID取得(キー情報のみ)
	const VecInt& GetID(std::string f_name,std::string key);								// ID取得(ハンドルネーム、キー情報)
	const VecInt& GetID(std::string f_name,std::string key,Vector2 divSize,Vector2 divCnt);	// ID取得(ハンドルネーム、キー情報、分割した際のサイズ、分割数)

private:
	ImageMng();
	~ImageMng();
	std::map<std::string, VecInt> imageMap_;				// 画像
};

