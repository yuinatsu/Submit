#include <DxLib.h>
#include "../ImageMng.h"
#include "MenuUI.h"

MenuUI::MenuUI(const Math::Vector2& pos, const std::string& imgName, int imgNum, float exRate) :
	pos_{pos}, imgName_{imgName}, imgNum_{imgNum}, exRate_{exRate}
{
	// 画像サイズを取得
	int x, y;
	GetGraphSize(lpImageMng.GetID(imgName_)[imgNum_], &x, &y);
	imgSize_ = { static_cast<float>(x) * exRate_, static_cast<float>(y)* exRate_ };
}

MenuUI::MenuUI(const MenuUI& menuUI) :
	pos_{menuUI.pos_}, imgName_{menuUI.imgName_}, imgSize_{menuUI.imgSize_}, imgNum_{menuUI.imgNum_}, exRate_{menuUI.exRate_}
{
}

MenuUI::~MenuUI()
{
	
}

void MenuUI::Draw(void)
{
	DrawRotaGraph(static_cast<int>(pos_.x), static_cast<int>(pos_.y), static_cast<double>(exRate_),0.0, lpImageMng.GetID(imgName_)[imgNum_], true);
}

const Math::Vector2 MenuUI::GetLT(void)
{
	// 左上を返す
	return pos_ - ((imgSize_/2.0f) + 5.0f);
}

const Math::Vector2 MenuUI::GetRB(void)
{
	// 右したを返す
	return pos_ + ((imgSize_ / 2.0f) + 5.0f);
}

void MenuUI::Relese(void)
{
	lpImageMng.Relese(imgName_);
}
