#pragma once
#include "Unit.h"

// ゲームクリア目標に当たるもの
class Target :
	public Unit
{
public:
	Target(std::vector<Math::Vector2I>& route,std::shared_ptr<TmxObj>& tmxObj,const Math::Vector2& pos);

	/// <summary> クリアしたか？ </summary>
	/// <param name=""> trueの時クリア </param>
	/// <returns></returns>
	const bool IsClear(void)const { return isClear_; }
private:

	void Draw(const Math::Vector2& offset, DrawMng& drawMng)override;
	ObjID GetObjID(void) override { return ObjID::Target;}

	/// <summary> 移動方向からアニメーションを切り替える </summary>
	/// <param name=""></param>
	void CorrectMoveVecToAnim(void) final;

	// クリアフラグ
	bool isClear_;
};

