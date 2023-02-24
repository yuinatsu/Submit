#pragma once
#include <memory>
#include "../NodeState.h"

class Obj;

// ビヘイビアベースAI用のノードの基底クラス
class BehaviorBaseNode
{
public:
	BehaviorBaseNode();
	virtual ~BehaviorBaseNode();

	/// <summary> リセット処理をかける </summary>
	/// <param name=""></param>
	virtual void Reset(void);

	/// <summary> 更新処理 </summary>
	/// <param name="obj"> 制御対象のオブジェクト </param>
	/// <param name="delta"> デルタタイム </param>
	/// <returns> ノードの状態 </returns>
	virtual NodeState operator()(Obj& obj, float delta) = 0;

	/// <summary> 自信をコピーする </summary>
	/// <param name=""></param>
	/// <returns></returns>
	virtual std::unique_ptr<BehaviorBaseNode> Copy(void) = 0;
};

