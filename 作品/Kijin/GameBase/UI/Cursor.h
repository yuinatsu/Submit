#pragma once
#include "../Common/Vector2.h"
#include "../UI/UiBase.h"

class BaseScene;

class Cursor :
	public UiBase
{
public:
	Cursor();
	void Check(std::list<std::unique_ptr<UiBase>>& uiList, BaseScene& scene, Controller& controller);
private:
	void Update(float delta, ObjectManager& objMng,Controller& controller) final;
	void Draw(void) final;
	UiID GetUIID(void)  { return UiID::Cursor; };
	SharedGraphicHandle handle_;
};

