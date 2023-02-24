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
	const VecInt& GetID(std::string key);
	const VecInt& GetID(std::string f_name,std::string key);
	const VecInt& GetID(std::string f_name,std::string key,Vector2 divSize,Vector2 divCnt);

private:
	ImageMng();
	~ImageMng();
	std::map<std::string, VecInt> imageMap_;
};

