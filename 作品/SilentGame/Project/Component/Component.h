#pragma once
#include <string>
#include "../common/Math.h"
#include "ComponentID.h"
class Obj;

/// <summary> オブジェクトに機能を持たせる用の基底クラス </summary>
class Component
{
public:

	/// <summary> コンストラクタ </summary>
	/// <param name="owner"> 持ち主のオブジェクト </param>
	Component(Obj& owner);

	/// <summary> デストラクタ </summary>
	virtual ~Component();

	/// <summary> 更新処理 </summary>
	/// <param name="delta"> デルタタイム </param>
	virtual void Update(float delta);

	virtual void Update(float delta, std::string key);

	virtual ComponentID GetID() const = 0;
#ifdef _DEBUG
	// デバッグ用描画
	virtual void DbgDraw(const Math::Vector2& offset) const {}
#endif
protected:

	/// <summary> 持ち主 </summary>
	Obj& owner_;

};

