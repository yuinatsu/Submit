#pragma once
#include "UiBase.h"
class Image :
    public UiBase
{
public:
	Image(const std::filesystem::path& path, const Vector2 pos);
private:
	void Update(float delta, ObjectManager& objMng, Controller& controller) final;
	void Draw();
	UiID GetUIID(void)
	{
		return UiID::Frame;
	}
private:
	SharedGraphicHandle handle_;
};

