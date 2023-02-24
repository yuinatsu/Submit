#include "SequenceNode.h"

SequenceNode::SequenceNode()
{
}

SequenceNode::~SequenceNode()
{
}

NodeState SequenceNode::operator()(Obj& obj, float delta)
{
	// ノードを実行して結果を保持
	auto result = (*(*nowItr_))(obj, delta);

	// 結果をチェックする
	if (result == NodeState::Success)
	{
		// 成功時

		// 末端まで行ったかをチェック
		if (++nowItr_ != children_.end())
		{
			// 末端まで言っていないとき実行中
			return (*this)(obj, delta);
		}
	}

	return result;
}

std::unique_ptr<BehaviorBaseNode> SequenceNode::Copy(void)
{
	std::unique_ptr<ParentNode> copy = std::make_unique<SequenceNode>();
	for (auto& c : children_)
	{
		copy->PushChildren(c->Copy());
	}
	return std::move(copy);
}
