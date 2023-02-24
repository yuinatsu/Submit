#pragma once
#include <map>
#include <list>
#include "../Obj.h"

class Effect :
    public Obj
{
public:
	/// <summary> コンストラクタ </summary>
	/// <param name="effect"> エフェクト名 </param>
	/// <param name="pos"> エフェクト発生座標 </param>
	Effect(EffectName effect, const Math::Vector2& pos);
	Effect(EffectName_x120 effect, const Math::Vector2& pos);
	Effect(EffectNameWhole effect, const Math::Vector2& pos);
	~Effect();
private:
	/// <summary> 更新 </summary>
	/// <param name="delta"> デルタタイム </param>
	void Update(float delta) override;

	/// <summary> 描画 </summary>
	/// <param name="offset"> 描画オフセット </param>
	/// <param name="drawMng"> 描画マネージャー </param>
	void Draw(const Math::Vector2& offset, DrawMng& drawMng) override;

	/// <summary> ID取得 </summary>
	/// <returns> ObjID::Effect </returns>
	ObjID GetObjID(void) override { return ObjID::Effect; };
};

