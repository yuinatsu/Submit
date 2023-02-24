#pragma once
#include <functional>
#include "BehaviorBaseNode.h"

// 成否を返す関数
using CheckFunc = std::function<bool(Obj&, float)>;

// コンディションノード(条件の結果によって成否を返す)
class ConditionNode :
	public BehaviorBaseNode
{
public:
	ConditionNode(const CheckFunc& checkFunc);
	~ConditionNode();
private:
	NodeState operator()(Obj& obj, float delta) override;

	// 成否を返す関数
	CheckFunc checkFunc_;
	std::unique_ptr<BehaviorBaseNode> Copy(void) override;
};

