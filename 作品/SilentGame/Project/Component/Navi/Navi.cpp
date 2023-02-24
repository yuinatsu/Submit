#include <algorithm>
#include "AstarNode.h"
#include "../../Resource/Tmx/TmxObj.h"
#include "../../Object/Obj.h"
#include "Navi.h"

#include "../../common/Debug.h"

Navi::Navi(std::shared_ptr<TmxObj>& tmxObj, Obj& owner) :
	Component{owner}, tmxObj_{tmxObj}
{
	
	state_ = NaviState::Non;

}

Navi::~Navi()
{
	if (GetNaviState() == NaviState::Search)
	{
		DebugLog("�X���b�h��~");
		workThread_.request_stop();
		if (workThread_.joinable())
		{
			workThread_.join();
			DebugLog("�X���b�hjoin����");
		}
	}
}

void Navi::Search(const Math::Vector2& start, const Math::Vector2I& goal)
{
	state_ = NaviState::Search;

	if (GetNaviState() == NaviState::Search)
	{
		// search�����������~����悤�ɂ���
		workThread_.request_stop();
	}

	goal_ = goal;
	// �T���֐�����Ɨp�X���b�h�ŊJ�n����
	workThread_ = std::jthread{ [this,start](std::stop_token token) { AstarSearch2(token,static_cast<Math::Vector2I>(start) / tmxObj_->GetTileSize(), goal_); } };
}

void Navi::Search(const Math::Vector2& start, const Math::Vector2& goal)
{
	Search(start, static_cast<Math::Vector2I>(goal) / tmxObj_->GetTileSize());
}

NaviState Navi::GetNaviState(void)
{
	std::unique_lock<std::mutex> ulock(mutex_, std::defer_lock);

	// ���b�N�ł��邩���`�F�b�N
	if (ulock.try_lock())
	{
		// ���b�N�ł���Ƃ�

		// �X���b�h���g���Ă��邩�`�F�b�N
		if (workThread_.joinable() && state_ == NaviState::Find)
		{
			// �������I����Ă���̂œ���
			workThread_.join();
		}
		return state_;
	}

	return NaviState::Search;

}


const Math::Vector2 Navi::GetNaviPos(void)
{
	std::lock_guard<std::mutex>lock(mutex_);
	return static_cast<Math::Vector2>(*nowNaviPos_ * tmxObj_->GetTileSize() + (tmxObj_->GetTileSize() / 2));
}

void Navi::Update(float delta)
{
	// �X�e�[�g�𒲂ׂ�
	if (GetNaviState() == NaviState::Find)
	{
		// �ړI�n�����鎞
		auto& tileSize = tmxObj_->GetTileSize();
		const auto& nowPos = static_cast<Math::Vector2I>(owner_.GetPos())/ tileSize;

		// GetNaviPos()/tileSize - owner_.GetPos();
		// ���݂̃}�X�ڂ��`�F�b�N
		if (nowPos == *nowNaviPos_)
		{
			
			// ���̃}�X�ڂ֍s�����ߐi�߂�
			++nowNaviPos_;
			if (nowNaviPos_ == destRoute_.end())
			{
				// ���[�̎������Ȃ̂ŏI��
				state_ = NaviState::Non;
				return;
			}
		}
	}
}


void Navi::AstarSearch2(std::stop_token token, const Math::Vector2I& start, const Math::Vector2I& goal)
{
	std::lock_guard<std::mutex> lock(mutex_);

	// �������Ă��錋�ʂ�������
	destRoute_.clear();

	// �X�^�[�g�ƃS�[����������
	if (start == goal)
	{
		// �����Ƃ��S�[�����������
		destRoute_.emplace_back(goal);
		nowNaviPos_ = destRoute_.begin();
		state_ = NaviState::Find;
		return;
	}

	// ���}�X�ڐ����擾
	const auto& num = tmxObj_->GetWorldArea();

	// �m�[�h�̏���������
	std::vector<std::vector<AstarNode>> nodeVec;
	nodeVec.resize(num.y);
	for (auto& vec : nodeVec)
	{
		vec.resize(num.x);
	}
	for (int y = 0; y < num.y; y++)
	{
		for (int x = 0; x < num.x; x++)
		{
			nodeVec[y][x].SetPos(Math::Vector2I{ x,y });

			if (tmxObj_->GetNaviFlag(x + 1, y))
			{
				nodeVec[y][x].PushN2(&nodeVec[y][x + 1]);
			}
			if (tmxObj_->GetNaviFlag(x - 1, y))
			{
				nodeVec[y][x].PushN2(&nodeVec[y][x - 1]);
			}
			if (tmxObj_->GetNaviFlag(x, y + 1))
			{
				nodeVec[y][x].PushN2(&nodeVec[y + 1][x]);
			}
			if (tmxObj_->GetNaviFlag(x, y - 1))
			{
				nodeVec[y][x].PushN2(&nodeVec[y - 1][x]);
			}
		}
	}


	// �\�[�g�p�����_
	auto sortFunc = [](AstarNode* nodeA, AstarNode* nodeB)
	{
		return nodeA->GetTotalCost() < nodeB->GetTotalCost();
	};

	// �I�[�v�����X�g
	std::list<AstarNode*> openList;

	// �N���[�Y���X�g
	std::list<AstarNode*> closeList;

	// �I�[�v�����X�g�ɃX�^�[�g�m�[�h������
	if (!tmxObj_->GetNaviFlag(start.x, start.y))
	{
		state_ = NaviState::Non;
		return;
	}
	openList.emplace_back(&nodeVec[start.y][start.x]);

	// �`�F�b�N����m�[�h
	AstarNode* node{nullptr};

	// ���ʂ��ꎞ�I�ɓ���Ă������߂̕ϐ�
	std::vector<std::vector<Math::Vector2I>> result;



	// ���ʗp�ϐ������T�C�Y���ď�����
	result.resize(num.y);
	for (auto& vec : result)
	{
		vec.resize(num.x);
		std::fill(vec.begin(), vec.end(), Math::Vector2I{ -1,-1 });
	}

	// �`�F�b�N�p�����_
	auto ckFunc = [](std::list<AstarNode*>& ckList, const AstarNode* node, int cost) {
		auto itr = std::find_if(
			ckList.begin(), ckList.end(),
			[&node](AstarNode* n) { return (n->GetPos() == node->GetPos()); }
		);

		// �����m�[�h�����邩�`�F�b�N
		if (itr != ckList.end())
		{
			// �����m�[�h�����鎞
			// �R�X�g���`�F�b�N
			if ((*itr)->GetTotalCost() > cost)
			{
				// ���X�g�ɂ�������R�X�g���������폜���ďI���
				ckList.erase(itr);
				return false;
			}
			return true;
		}
		return false;
	};

	while (!openList.empty())
	{
		if (token.stop_requested())
		{
			// ��~���N�G�X�g���������߂�
			state_ = NaviState::Non;
			return;
		}
		// ���X�g�̈�Ԑ擪���擾
		node = *openList.begin();

		// �擾��������폜
		openList.pop_front();

		// �S�[�����ǂ����`�F�b�N
		if (node->GetPos() == goal)
		{
			// �S�[���̎��N���[�Y���X�g�ɓ���ă��[�v�𔲂���
			closeList.emplace_back(node);
			break;
		}

		// �אڃm�[�h���I�[�v�����X�g�ɓ����
		for (auto& next : node->GetN())
		{

			// �q���[���X�e�B�b�N�R�X�g���v�Z
			next->CalculateHeuristicCost(goal);

			// �V�����R�X�g���v�Z
			int ncost = 1 + next->GetHeuristicCost() + node->GetTotalCost();

			// ���X�g�ɓ����m�[�h�����݂��ă��X�g�̕����R�X�g�������ꍇ�폜

			// ���X�g���`�F�b�N
			if (!ckFunc(closeList, next, ncost) && !ckFunc(openList, next, ncost))
			{
				// �����m�[�h�������������͂����Ă��V�����R�X�g���Ⴂ�Ƃ��ǉ�
				next->SetTotalCost(ncost);
				result[next->GetPos().y][next->GetPos().x] = node->GetPos();
				openList.emplace_back(next);
			}

		}

		// �N���[�Y���X�g�ɓ����m�[�h�����邩�T��
		auto itr = std::find_if(closeList.begin(), closeList.end(), [&node](AstarNode* n) { return n->GetPos() == node->GetPos(); });

		if (itr == closeList.end())
		{
			// �����m�[�h���Ȃ��Ƃ��m�[�h�̒T�����I���Ȃ̂ŃN���[�Y���X�g�ɓ����
			closeList.emplace_back(node);
		}
		// �I�[�v�����X�g���\�[�g����
		openList.sort(sortFunc);


	}

	// ���ʂ��i�[����
	destRoute_.emplace_back(goal);
	auto& p = result[goal.y][goal.x];
	while (true)
	{

		if (p == start)
		{
			destRoute_.emplace_back(p);
			state_ = NaviState::Find;
			break;
		}
		else if (p == Math::Vector2I{ -1,-1 })
		{
			state_ = NaviState::Non;
			destRoute_.clear();
			destRoute_.emplace_back(start);
			break;
		}
		destRoute_.emplace_back(p);
		p = result[p.y][p.x];
	}

	destRoute_.reverse();


	// �擪�����Ƃ�
	nowNaviPos_ = destRoute_.begin();
}

ComponentID Navi::GetID() const
{
	return ComponentID::Navi;
}

