#pragma once
#include <map>
#include <array>
#include <string>
#include "InputID.h"

// トリガー情報
enum class Trg
{
	Now,				// 現在
	Old,				// 一つ前
	Max
};

// 入力装置の種類
enum class CntType
{
	Key,				// キーボード
	Max
};

using inputData = std::array<std::pair<bool, bool>, static_cast<size_t>(InputID::Max) + 1>;

class Controller
{
public:
	bool operator()(double delta)
	{
		if (this != nullptr)
		{
			Update(delta);
		}
		return this != nullptr;
	}
	Controller();
	virtual ~Controller() {}
	// 更新
	virtual void Update(double delta) = 0;
	// コントローラーの種類の取得
	virtual CntType GetCntType(void) = 0;

	// 今押しているか
	bool Press(InputID id);
	// 押した瞬間
	bool Pressed(InputID id);
	// 離した瞬間
	bool Released(InputID id);
protected:
	// 入力情報
	inputData data_;
};

