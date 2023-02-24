#pragma once
#include "ParentNode.h"

// �V�[�N�G���X�m�[�h(�����Ă���m�[�h�����s����܂ł��ׂĎ��s����)
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

