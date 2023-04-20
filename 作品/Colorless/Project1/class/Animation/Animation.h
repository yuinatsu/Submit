#pragma once
#include <map>
#include <vector>
#include <string>
#include "../common/Vector2.h"

using AnimVector = std::vector<std::pair<int, int>>;

// ステータスの種類
enum class State
{
	up,
	left,
	right,
	down,
	MAX
};

class Animation
{
public:
	Animation(std::string key);
	~Animation();
	bool Init(void);											// 初期化
	bool Update(void);											// 更新
	bool Draw(Vector2F pos,Vector2 size,float mag = 1.0f);		// 描画 Draw(座標, サイズ, 倍率)
	bool state(const std::string state);						// ステータス state(ステータス名)
	std::string GetKey(void);									// キー情報取得
	std::string GetState(void);									// ステータス取得
private:
	std::string key_;											// キー
	std::string state_;											// 状態
	int animFrame_;												// アニメーションフレーム
	int animCount_;												// アニメーションカウンタ
};

