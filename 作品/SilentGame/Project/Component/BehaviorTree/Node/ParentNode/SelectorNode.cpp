#include "SelectorNode.h"

SelectorNode::SelectorNode()
{
}

SelectorNode::~SelectorNode()
{
}

NodeState SelectorNode::operator()(Obj& obj, float delta)
{
	// 実行して結果を保持
	auto result = (*(*nowItr_))(obj, delta);

	// 結果をチェックする
	if (result == NodeState::Faild)
	{
		// 失敗時

		// イテレーターを進めて末端かチェック
		if (++nowItr_ == children_.end())
		{
			// 末端まで行った時失敗
			return NodeState::Faild;
		}

		// 末端まで言ってないのでまだ処理を続ける
		result = (*this)(obj,delta);
	}

	return result;
}

std::unique_ptr<BehaviorBaseNode> SelectorNode::Copy(void)
{
	std::unique_ptr<ParentNode> copy = std::make_unique<SelectorNode>();
	for (auto& c : children_)
	{
		copy->PushChildren(c->Copy());
	}
	return std::move(copy);
}
