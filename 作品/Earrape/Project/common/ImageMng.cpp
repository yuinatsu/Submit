#include <DxLib.h>
#include "ImageMng.h"
#include "../_debug/_DebugConOut.h"

const VecInt& ImageMng::GetID(std::string key)
{
	if (imgMap_.find(key) == imgMap_.end())
	{
		imgMap_[key].resize(1);
		imgMap_[key][0] = LoadGraph(key.c_str());
	}
	return imgMap_[key];
}

const VecInt& ImageMng::GetID(std::string f_name, std::string key, Vector2 divSize, Vector2 divCnt)
{
	if (imgMap_.find(key) == imgMap_.end())
	{
		TRACE("ƒtƒ@ƒCƒ‹‚Ì“Ç‚İ‚İ\n");
		imgMap_[key].resize(divCnt.x * divCnt.y);

		// ‰æ‘œ‚ğ“Ç‚İ‚ñ‚Å‚¢‚È‚¢ê‡“Ç‚İ‚Ş
		LoadDivGraph(f_name.c_str(), (divCnt.x * divCnt.y), divCnt.x, divCnt.y, divSize.x, divSize.y, &imgMap_[key][0], true);
	}
	return imgMap_[key];
}

ImageMng::ImageMng()
{
}

ImageMng::~ImageMng()
{
}
