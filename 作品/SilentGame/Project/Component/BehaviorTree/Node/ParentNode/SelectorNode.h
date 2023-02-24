#pragma once
#include "ParentNode.h"

// セレクターノード(持っているノードを先頭から実行して成功した時点で完了する)
class SelectorNode :
	public ParentNode
{
public:
	SelectorNode();
	~SelectorNode();
private:
	NodeState operator()(Obj& obj, float delta) override;

	std::unique_ptr<BehaviorBaseNode> Copy(void) override;
};

