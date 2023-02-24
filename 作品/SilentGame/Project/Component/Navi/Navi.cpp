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
		DebugLog("スレッド停止");
		workThread_.request_stop();
		if (workThread_.joinable())
		{
			workThread_.join();
			DebugLog("スレッドjoin完了");
		}
	}
}

void Navi::Search(const Math::Vector2& start, const Math::Vector2I& goal)
{
	state_ = NaviState::Search;

	if (GetNaviState() == NaviState::Search)
	{
		// search中だったら停止するようにする
		workThread_.request_stop();
	}

	goal_ = goal;
	// 探索関数を作業用スレッドで開始する
	workThread_ = std::jthread{ [this,start](std::stop_token token) { AstarSearch2(token,static_cast<Math::Vector2I>(start) / tmxObj_->GetTileSize(), goal_); } };
}

void Navi::Search(const Math::Vector2& start, const Math::Vector2& goal)
{
	Search(start, static_cast<Math::Vector2I>(goal) / tmxObj_->GetTileSize());
}

NaviState Navi::GetNaviState(void)
{
	std::unique_lock<std::mutex> ulock(mutex_, std::defer_lock);

	// ロックできるかをチェック
	if (ulock.try_lock())
	{
		// ロックできるとき

		// スレッドが使われているかチェック
		if (workThread_.joinable() && state_ == NaviState::Find)
		{
			// 処理が終わっているので同期
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
	// ステートを調べる
	if (GetNaviState() == NaviState::Find)
	{
		// 目的地がある時
		auto& tileSize = tmxObj_->GetTileSize();
		const auto& nowPos = static_cast<Math::Vector2I>(owner_.GetPos())/ tileSize;

		// GetNaviPos()/tileSize - owner_.GetPos();
		// 現在のマス目をチェック
		if (nowPos == *nowNaviPos_)
		{
			
			// 次のマス目へ行くため進める
			++nowNaviPos_;
			if (nowNaviPos_ == destRoute_.end())
			{
				// 末端の時到着なので終了
				state_ = NaviState::Non;
				return;
			}
		}
	}
}


void Navi::AstarSearch2(std::stop_token token, const Math::Vector2I& start, const Math::Vector2I& goal)
{
	std::lock_guard<std::mutex> lock(mutex_);

	// 今持っている結果を初期化
	destRoute_.clear();

	// スタートとゴールが同じか
	if (start == goal)
	{
		// 同じときゴールだけ入れる
		destRoute_.emplace_back(goal);
		nowNaviPos_ = destRoute_.begin();
		state_ = NaviState::Find;
		return;
	}

	// 総マス目数を取得
	const auto& num = tmxObj_->GetWorldArea();

	// ノードの情報を初期化
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


	// ソート用ラムダ
	auto sortFunc = [](AstarNode* nodeA, AstarNode* nodeB)
	{
		return nodeA->GetTotalCost() < nodeB->GetTotalCost();
	};

	// オープンリスト
	std::list<AstarNode*> openList;

	// クローズリスト
	std::list<AstarNode*> closeList;

	// オープンリストにスタートノードを入れる
	if (!tmxObj_->GetNaviFlag(start.x, start.y))
	{
		state_ = NaviState::Non;
		return;
	}
	openList.emplace_back(&nodeVec[start.y][start.x]);

	// チェックするノード
	AstarNode* node{nullptr};

	// 結果を一時的に入れておくための変数
	std::vector<std::vector<Math::Vector2I>> result;



	// 結果用変数をリサイズして初期化
	result.resize(num.y);
	for (auto& vec : result)
	{
		vec.resize(num.x);
		std::fill(vec.begin(), vec.end(), Math::Vector2I{ -1,-1 });
	}

	// チェック用ラムダ
	auto ckFunc = [](std::list<AstarNode*>& ckList, const AstarNode* node, int cost) {
		auto itr = std::find_if(
			ckList.begin(), ckList.end(),
			[&node](AstarNode* n) { return (n->GetPos() == node->GetPos()); }
		);

		// 同じノードがあるかチェック
		if (itr != ckList.end())
		{
			// 同じノードがある時
			// コストをチェック
			if ((*itr)->GetTotalCost() > cost)
			{
				// リストにある方がコストが高い時削除して終わる
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
			// 停止リクエストがきたらやめる
			state_ = NaviState::Non;
			return;
		}
		// リストの一番先頭を取得
		node = *openList.begin();

		// 取得したやつを削除
		openList.pop_front();

		// ゴールかどうかチェック
		if (node->GetPos() == goal)
		{
			// ゴールの時クローズリストに入れてループを抜ける
			closeList.emplace_back(node);
			break;
		}

		// 隣接ノードをオープンリストに入れる
		for (auto& next : node->GetN())
		{

			// ヒューリスティックコストを計算
			next->CalculateHeuristicCost(goal);

			// 新しいコストを計算
			int ncost = 1 + next->GetHeuristicCost() + node->GetTotalCost();

			// リストに同じノードが存在してリストの方がコストが高い場合削除

			// リストをチェック
			if (!ckFunc(closeList, next, ncost) && !ckFunc(openList, next, ncost))
			{
				// 同じノードが無いもしくはあっても新しいコストが低いとき追加
				next->SetTotalCost(ncost);
				result[next->GetPos().y][next->GetPos().x] = node->GetPos();
				openList.emplace_back(next);
			}

		}

		// クローズリストに同じノードがあるか探す
		auto itr = std::find_if(closeList.begin(), closeList.end(), [&node](AstarNode* n) { return n->GetPos() == node->GetPos(); });

		if (itr == closeList.end())
		{
			// 同じノードがないときノードの探索を終了なのでクローズリストに入れる
			closeList.emplace_back(node);
		}
		// オープンリストをソートする
		openList.sort(sortFunc);


	}

	// 結果を格納する
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


	// 先頭を入れとく
	nowNaviPos_ = destRoute_.begin();
}

ComponentID Navi::GetID() const
{
	return ComponentID::Navi;
}

