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
	// �X�V���ʂ��`�F�b�N����
	if ((*rootNode_)(owner_, delta) != NodeState::Now)
	{
		// ���s���ł͂Ȃ��Ƃ����Z�b�g��������
		rootNode_->Reset();
	}
}

ComponentID BehaviorTree::GetID() const
{
	return ComponentID::BehaviorTree;
}
