#pragma once
#include "ParentNode.h"

// �Z���N�^�[�m�[�h(�����Ă���m�[�h��擪������s���Đ����������_�Ŋ�������)
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

