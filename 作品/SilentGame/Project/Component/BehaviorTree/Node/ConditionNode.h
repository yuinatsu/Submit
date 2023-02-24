#pragma once
#include <functional>
#include "BehaviorBaseNode.h"

// ���ۂ�Ԃ��֐�
using CheckFunc = std::function<bool(Obj&, float)>;

// �R���f�B�V�����m�[�h(�����̌��ʂɂ���Đ��ۂ�Ԃ�)
class ConditionNode :
	public BehaviorBaseNode
{
public:
	ConditionNode(const CheckFunc& checkFunc);
	~ConditionNode();
private:
	NodeState operator()(Obj& obj, float delta) override;

	// ���ۂ�Ԃ��֐�
	CheckFunc checkFunc_;
	std::unique_ptr<BehaviorBaseNode> Copy(void) override;
};

