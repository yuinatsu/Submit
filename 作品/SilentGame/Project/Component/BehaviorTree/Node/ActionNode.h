#pragma once
#include <functional>
#include "BehaviorBaseNode.h"

using ActFunc = std::function<NodeState(Obj&, float)>;

// アクションノード(末端で何かを実行する)
class ActionNode :
	public BehaviorBaseNode
{
public:
	ActionNode(const ActFunc& actFunc);
	~ActionNode();
private:
	// 実行する関数
	ActFunc actFunc_;

	/// <summary> 持っているfunctionを実行する </summary>
	/// <param name="obj"> obj </param>
	/// <param name="delta"> デルタタイム </param>
	/// <returns></returns>
	NodeState operator()(Obj& obj, float delta) override;

	/// <summary> 自信をコピーする </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::unique_ptr<BehaviorBaseNode> Copy(void) override;
};

