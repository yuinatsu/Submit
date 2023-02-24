#include "Node/BehaviorBaseNode.h"
#include "BehaviorTree.h"

BehaviorTree::BehaviorTree(std::unique_ptr<BehaviorBaseNode> node, Obj& owner) :
	Component{owner} , rootNode_{std::move(node)}
{
	rootNode_->Reset();
}

BehaviorTree::~BehaviorTree()
{
}

void BehaviorTree::Reset(void)
{
	rootNode_->Reset();
}

void BehaviorTree::Update(float delta)
{
	// 更新結果をチェックする
	if ((*rootNode_)(owner_, delta) != NodeState::Now)
	{
		// 実行中ではないときリセットをかける
		rootNode_->Reset();
	}
}

ComponentID BehaviorTree::GetID() const
{
	return ComponentID::BehaviorTree;
}
