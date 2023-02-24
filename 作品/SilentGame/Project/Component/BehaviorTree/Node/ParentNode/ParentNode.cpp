#include "ParentNode.h"

ParentNode::ParentNode()
{
}

ParentNode::~ParentNode()
{
}

bool ParentNode::PushChildren(std::unique_ptr<BehaviorBaseNode>&& child)
{
	children_.emplace_back(std::move(child));
	return true;
}

void ParentNode::Reset(void)
{
	for (auto& child : children_)
	{
		child->Reset();
	}
	// 先頭の子ノードをセットする
	nowItr_ = children_.begin();
}
