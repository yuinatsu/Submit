#include <numeric>
#include "AstarNode.h"

AstarNode::AstarNode(const Math::Vector2I& pos) :
	pos_{pos}
{
	Clear();
}

AstarNode::AstarNode()
{
	Clear();
}

void AstarNode::PushNextNode(std::shared_ptr<AstarNode>& node)
{
	nextNode_.emplace_back(node);
}

void AstarNode::Clear()
{
	heuristicCost_ = std::numeric_limits<int>::max();
	totalCost_ = 0;
	beforPos_ = { -1,-1 };
}

const int AstarNode::GetTotalCost(void) const
{
	return totalCost_;
}

void AstarNode::SetTotalCost(int cost)
{
	totalCost_ = cost;
}

const int AstarNode::GetHeuristicCost(void) const
{
	return heuristicCost_;
}

void AstarNode::CalculateHeuristicCost(const Math::Vector2I& pos)
{
	if (heuristicCost_ == std::numeric_limits<int>::max())
	{
		heuristicCost_ = (std::abs(pos_.x - pos.x) + std::abs(pos_.y - pos.y));
	}
}

const Math::Vector2I& AstarNode::GetPos(void) const
{
	return pos_;
}


