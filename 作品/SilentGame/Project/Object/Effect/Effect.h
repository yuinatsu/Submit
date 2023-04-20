#pragma once
#include <map>
#include <list>
#include "../Obj.h"

class Effect :
    public Obj
{
public:
	// コンストラクタ 
	Effect(EffectName effect, const Math::Vector2& pos);
	Effect(EffectName_x120 effect, const Math::Vector2& pos);
	Effect(EffectNameWhole effect, const Math::Vector2& pos);
	~Effect();
private:
	// 更新 
	void Update(float delta) override;

	// 描画 
	void Draw(const Math::Vector2& offset, DrawMng& drawMng) override;

	// ID取得 
	ObjID GetObjID(void) override { return ObjID::Effect; };
};

