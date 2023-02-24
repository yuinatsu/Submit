#include "SequenceNode.h"

SequenceNode::SequenceNode()
{
}

SequenceNode::~SequenceNode()
{
}

NodeState SequenceNode::operator()(Obj& obj, float delta)
{
	// �m�[�h�����s���Č��ʂ�ێ�
	auto result = (*(*nowItr_))(obj, delta);

	// ���ʂ��`�F�b�N����
	if (result == NodeState::Success)
	{
		// ������

		// ���[�܂ōs���������`�F�b�N
		if (++nowItr_ != children_.end())
		{
			// ���[�܂Ō����Ă��Ȃ��Ƃ����s��
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
