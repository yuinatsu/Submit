#pragma once
#include <string>
#include <list>

// 格納用データ
struct AnimationData
{
	int loop;		// ループ回数
	std::list < std::pair<int, float>> dataList;		// アニメーションデータマス目と表示秒数
};

