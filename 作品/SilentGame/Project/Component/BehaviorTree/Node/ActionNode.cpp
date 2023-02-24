#include "ActionNode.h"

ActionNode::ActionNode(const ActFunc& actFunc) :
	actFunc_{actFunc}
{
}

ActionNode::~ActionNode()
{
}

NodeState ActionNode::operator()(Obj& obj, float delta)
{
	return actFunc_(obj,delta);
}

std::unique_ptr<BehaviorBaseNode> ActionNode::Copy(void)
{
	return std::make_unique<ActionNode>(actFunc_);
}
