#pragma once
#include "ParentNode.h"

// シークエンスノード(持っているノードを失敗するまですべて実行する)
class SequenceNode :
	public ParentNode
{
public:
	SequenceNode();
	~SequenceNode();
private:
	NodeState operator()(Obj& obj, float delta) override;
	std::unique_ptr<BehaviorBaseNode> Copy(void) override;
};

