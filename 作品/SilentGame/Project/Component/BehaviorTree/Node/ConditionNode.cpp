#include "ConditionNode.h"

ConditionNode::ConditionNode(const CheckFunc& checkFunc) :
	checkFunc_{checkFunc}
{
}

ConditionNode::~ConditionNode()
{
}

NodeState ConditionNode::operator()(Obj& obj, float delta)
{
	// •Ô’l‚ªtrue‚Ì¬Œ÷false‚Ì¸”s‚ğ•Ô‚·
	return checkFunc_(obj,delta) == true ? NodeState::Success: NodeState::Faild;
}

std::unique_ptr<BehaviorBaseNode> ConditionNode::Copy(void)
{
	return std::make_unique<ConditionNode>(checkFunc_);
}
