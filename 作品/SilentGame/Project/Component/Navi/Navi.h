#pragma once
#include <memory>
#include <list>
#include <mutex>
#include <thread>
#include <stop_token>
#include <functional>
#include "../../common/Math.h"
#include "../Component.h"


class TmxObj;

// Naviの状態
enum class NaviState
{
	Search,		// 探索中
	Find,		// 目的地が存在するとき
	Non,		// 何もしない止まっとく
};

// 経路探索用クラス
class Navi :
	public Component
{
	// 座標のリスト
	using Vector2lVec = std::list<Math::Vector2I>;

	// 座標のリストのイテレーター
	using Vector2lVecItr = Vector2lVec::iterator;


	using FuncListItr = std::list<std::function<void(void)>>::const_iterator;

public:
	Navi(std::shared_ptr<TmxObj>& tmxObj,Obj& owner);
	~Navi();

	/// <summary> スタートからゴールまでの経路の探索 </summary>
	/// <param name="start"> スタート地点の座標 </param>
	/// <param name="goal"> ゴール地点の升目 </param>
	void Search(const Math::Vector2& start, const Math::Vector2I& goal);

	void Search(const Math::Vector2& start, const Math::Vector2& goal);

	/// <summary> Naviクラスの状態を取得する </summary>
	/// <param name=""></param>
	/// <returns> Naviクラスの状態 </returns>
	NaviState GetNaviState(void);

	/// <summary> 次行くべき座標を取得 </summary>
	/// <param name=""></param>
	/// <returns> 次行くべき座標(Vector2) </returns>
	const Math::Vector2 GetNaviPos(void);
private:


	void Update(float delta) override;

	
	/// <summary> A*法を使って経路探索を行う </summary>
	/// <param name="start"> 開始マス目 </param>
	/// <param name="goal"> 目的のマス目 </param>
	void AstarSearch2(std::stop_token token, const Math::Vector2I& start, const Math::Vector2I& goal);

	ComponentID GetID() const override;

	Math::Vector2I goal_;

	// 現在の目的地のマス目
	Vector2lVecItr nowNaviPos_;


	// 探査結果
	Vector2lVec destRoute_;

	// マップに関するTmxオブジェクト
	std::shared_ptr<TmxObj> tmxObj_;

	// 探索作業を行うスレッド
	std::jthread workThread_;

	// ミューテックス
	std::mutex mutex_;

	// 状態
	NaviState state_;
};

