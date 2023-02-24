#pragma once
#include <memory>
#include <list>
#include "../../common/Math.h"

class AstarNode
{
public:
	AstarNode(const Math::Vector2I& pos);
	AstarNode();
	void PushNextNode(std::shared_ptr<AstarNode>& node);
	void Clear();

	const int GetTotalCost(void) const;
	void SetTotalCost(int cost);

	const int GetHeuristicCost(void) const;
	void CalculateHeuristicCost(const Math::Vector2I& pos);

	const std::list<std::weak_ptr<AstarNode>>& GetNextNode(void) const { return nextNode_; };

	const Math::Vector2I& GetPos(void) const;
	void SetPos(const Math::Vector2I& pos) { pos_ = pos; }

	const std::list<AstarNode*> GetN(void) const { return nextNode2_; }

	void PushN2(AstarNode* node) { nextNode2_.emplace_back(node); }

private:

	// ���W(�}�X��)
	Math::Vector2I pos_;

	Math::Vector2I beforPos_;

	// �אڃm�[�h
	std::list<std::weak_ptr<AstarNode>> nextNode_;

	std::list<AstarNode*> nextNode2_;

	// �q���[���X�e�B�b�N�R�X�g(�S�[���܂ł̋������R�X�g��)
	int heuristicCost_;

	// ���v�R�X�g
	int totalCost_;

};

