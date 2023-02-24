#include <DxLib.h>
#include <algorithm>
#include "DrawMng.h"
#include "../Scene/SceneMng.h"
#include "Debug.h"

constexpr float posMin{ -16.0f };

// 範囲チェックする
constexpr bool CheckDrawPos(const Math::Vector2& pos)
{
	// 範囲外の時true
	return pos.x < posMin || pos.x > static_cast<float>(lpSceneMng.GetScreenSize().x) || pos.y < posMin || pos.y > static_cast<float>(lpSceneMng.GetScreenSize().y);
}

DrawMng::DrawMng(float maxPosY) :
	maxPosY_{maxPosY}
{
	queueCount_ = 0;
}

DrawMng::~DrawMng()
{
}

void DrawMng::Draw(const Math::Vector2& pos, int handle)
{
	if (CheckDrawPos(pos))
	{
		// 範囲外の時描画を追加しない
		return;
	}
	Draw([this, pos, handle]() { DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), handle, true); }, GetPosY2Depth(pos.y));
}

void DrawMng::Draw(const Math::Vector2& pos, float rot, int handle)
{
	if (CheckDrawPos(pos))
	{
		// 範囲外の時描画を追加しない
		return;
	}
	Draw([this, pos, rot, handle]() { DrawRotaGraph(static_cast<int>(pos.x), static_cast<int>(pos.y),1.0, rot, handle, true); }, GetPosY2Depth(pos.y));
}

void DrawMng::Draw(const Math::Vector2& pos, float depth, float rot, int handle)
{
	// 描画範囲チェック
	if (CheckDrawPos(pos))
	{
		// 範囲外の時描画を追加しない
		return;
	}

	Draw([this, pos, rot,handle]() { DrawRotaGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), 1.0, rot, handle, true); }, depth);
}

void DrawMng::Draw(void)
{
	// 終端に当たるイテレーターを取得
	auto end = (drawQueue_.begin() + queueCount_);

	// 深度の大きい順にソート
	std::sort(drawQueue_.begin(), end, [](auto& a, auto& b) {return a.second > b.second; });

	// 順番に描画する(値的に奥にあるものから)
	for (int i = 0; i < queueCount_; i++)
	{
		drawQueue_[i].first();
	}

	// キューをclear
	queueCount_ = 0;
}

void DrawMng::Draw(std::function<void(void)>&& func, float depth)
{
	// 深度をクランプする
	depth = Math::Clamp(depth, 0.0f, 1.0f);

	// queueのチェック
	if (queueCount_ < drawQueue_.size())
	{
		// queueの数が最大サイズより小さいとき順番にデータを入れる
		drawQueue_[queueCount_] = std::make_pair(
			func,
			depth
		);
	}
	else
	{
		// そうでないとき後ろから新たに追加していく
		drawQueue_.emplace_back(
			std::make_pair(
				func,
				depth
			)
		);
	}
	queueCount_++;
}
