#include "SelectorNode.h"

SelectorNode::SelectorNode()
{
}

SelectorNode::~SelectorNode()
{
}

NodeState SelectorNode::operator()(Obj& obj, float delta)
{
	// ���s���Č��ʂ�ێ�
	auto result = (*(*nowItr_))(obj, delta);

	// ���ʂ��`�F�b�N����
	if (result == NodeState::Faild)
	{
		// ���s��

		// �C�e���[�^�[��i�߂Ė��[���`�F�b�N
		if (++nowItr_ == children_.end())
		{
			// ���[�܂ōs���������s
			return NodeState::Faild;
		}

		// ���[�܂Ō����ĂȂ��̂ł܂������𑱂���
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
