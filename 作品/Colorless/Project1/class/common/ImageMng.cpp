#include <DxLib.h>
#include "ImageMng.h"

const VecInt& ImageMng::GetID(std::string key)
{
	return GetID(key,key);
}

const VecInt& ImageMng::GetID(std::string f_name, std::string key)
{
	if (imageMap_.find(key) == imageMap_.end())
	{
		imageMap_[key].resize(1);
		imageMap_[key][0] = LoadGraph(f_name.c_str());
	}
	return imageMap_[key];
}

const VecInt& ImageMng::GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt)
{
	if (imageMap_.find(key) == imageMap_.end())
	{
		imageMap_[key].resize(static_cast<__int64>(divCnt.x) * divCnt.y);

		LoadDivGraph(f_name.c_str(), divCnt.x * divCnt.y, divCnt.x, divCnt.y, divSize.x,divSize.y, &imageMap_[key][0]);
	}
	return imageMap_[key];
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}
