#pragma once
#include <map>
#include <array>
#include <string>
#include "InputID.h"
#include "../Vector2.h"
#include "../../SceneManager.h"

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
	Pad,				// パッド
	Max
};

using InputData = std::array<std::pair<bool, bool>, static_cast<size_t>(InputID::Max) + 1>;

class Controller
{
public:
	bool operator()(float delta)
	{
		if (this != nullptr)
		{
			Update(delta);
		}
		return this != nullptr;
	}
	Controller();
	virtual ~Controller();

	// 初期化 
	virtual bool Init(void) = 0;

	// 更新 
	virtual void Update(float delta) = 0;

	// コントローラーの種類の取得 
	virtual CntType GetCntType(void) = 0;

	// 今左クリックしているか 
	//bool MousePress(InputID id);

	// 今押しているか 
	bool Press(InputID id);

	// 押した瞬間 
	bool Pressed(InputID id);

	// 離した瞬間 
	bool Released(InputID id);

	// 今押されていないとき 
	bool NotPress(InputID id);

	// 今何も押されていないか 
	bool IsNeutral();

	// カーソル位置をセット(デフォルト引数は中心) 
	virtual void SetCursorPos(const Vector2& pos = lpSceneMng.screenSize_<float> / 2.0f) = 0;

	// カーソル位置の取得 
	const Vector2& GetCursorPos(void) const&
	{
		return cursorPos_;
	}

	// カーソルのスピードをセット 
	void SetCursorSpeed(float speed)
	{
		cursorSpeed_ = speed;
	}

	// 左(移動)用 
	const Vector2& GetLeftInput(void) const&
	{
		return leftInput_;
	}

	// 視点移動に使う座標を取得 
	const Vector2& GetRightInput(void) const&
	{
		return rightInput_;
	}

	// 決定ボタンが押されているか 
	bool PressDecision(void) const;

	// 決定ボタンが押された瞬間か 
	bool PressedDecision(void) const;

	// キャンセルボタンが押されたか 
	bool PressCancel(void) const;

	// キャンセルボタンが押された瞬間か 
	bool PressdCancel(void) const;
protected:
	// 入力情報 
	InputData data_;

	// 決定下かの情報の情報
	std::pair<bool, bool> decisionData_;

	// キャンセル押したかのデータ
	std::pair<bool, bool> cancelData_;

	// カーソルの座標
	Vector2 cursorPos_;

	// カーソルのスピード(ui時に使用)
	float cursorSpeed_;

	// 移動用の入力された方向
	Vector2 leftInput_;

	// 振り向き用入力座標
	Vector2 rightInput_;
};

