#pragma once
#include "UiBase.h"

class BaseScene;

// カーソルでクリックできるUiの基底クラス
class ButtonBase :
	public UiBase
{
public:
	ButtonBase(const Vector2& pos);
	virtual ~ButtonBase();

	/// <summary>
	/// 押された時呼ばれる
	/// </summary>
	/// <param name="scene"> シーン </param>
	virtual void Push(BaseScene& scene, Controller& controller) = 0;

	/// <summary>
	/// カーソルと当たった瞬間呼ばれる
	/// </summary>
	/// <param name=""></param>
	virtual void HitCursor(void) = 0;

	/// <summary>
	/// カーソルと当たらなくなった瞬間呼ばれる
	/// </summary>
	/// <param name=""></param>
	virtual void NotHitCursor(void) = 0;

	/// <summary>
	/// カーソルと当たっているか
	/// </summary>
	/// <param name=""></param>
	/// <returns> 当たっているときtrue </returns>
	bool IsHitCursor(void) const
	{
		return isHitCursor_;
	}


	bool IsButton(void) const final
	{
		return true;
	}

	// カーソルと当たっている時
	bool isHitCursor_;
};

