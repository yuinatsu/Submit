#include "Image.h"
#include <DxLib.h>
#include "../Common/ResourceMng.h"
#include "../Application.h"

Image::Image(const std::filesystem::path& path, const Vector2 pos) :
	UiBase{pos}
{
	lpResourceMng.LoadTexture(handle_, path);
}

void Image::Update(float delta, ObjectManager& objMng, Controller& controller)
{
}

void Image::Draw()
{
	DrawRotaGraphF(pos_.x, pos_.y, 1.0f, 0.0f, *handle_, true, false);
	//DrawGraph(static_cast<int>(pos_.x), static_cast<int>(pos_.y), *handle_, true);
}
