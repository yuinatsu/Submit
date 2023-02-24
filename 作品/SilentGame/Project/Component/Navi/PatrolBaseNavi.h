#pragma once
#include <memory>
#include <vector>
#include "../Component.h"
#include "../../common/Math.h"

class TmxObj;

class PatrolBaseNavi :
	public Component
{
	using Vector2lVec = std::vector<Math::Vector2I>;
public:
	PatrolBaseNavi(Vector2lVec& route, std::shared_ptr<TmxObj>& tmxObj, Obj& owner);
	virtual ~PatrolBaseNavi();

	/// <summary> 次行くべき座標を取得する </summary>
	/// <param name=""></param>
	/// <returns> 座標 </returns>
	virtual const Math::Vector2 GetMovePos(void) const = 0;

	/// <summary> 現在のマス目 </summary>
	/// <param name=""></param>
	/// <returns></returns>
	virtual const Math::Vector2I& GetPos(void)const = 0;

	/// <summary> 停止しているか </summary>
	/// <param name=""></param>
	/// <returns> 停止しているときtrueしていないときfalse </returns>
	bool IsStop(void) const
	{
		return stopFlag_;
	}

	/// <summary> 停止させる </summary>
	/// <param name=""></param>
	void Stop(void)
	{
		stopFlag_ = true;
	}

	/// <summary> 巡回を開始する </summary>
	/// <param name=""></param>
	void Start(void)
	{
		stopFlag_ = false;
	}

protected:
	

	ComponentID GetID() const
	{
		return ComponentID::PatrolNavi;
	}


	// マップ情報
	std::shared_ptr<TmxObj> tmxObj_;


	// 巡回用ルート
	Vector2lVec route_;

	
	// 停止フラグ
	bool stopFlag_;
};

