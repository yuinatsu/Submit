#pragma once
#include <type_traits>

// キー情報
enum class InputID
{
	Up,						// 上移動
	Down,					// 下移動
	Left,					// 左移動
	Right,					// 右移動
	CameraUp,				// カメラ上移動
	CameraDown,				// カメラ下移動
	CameraLeft,				// カメラ左移動
	CameraRight,			// カメラ右移動
	Dash,					// ダッシュ
	Dimension,				// 2次元操作変更
	Decision,				// 決定
	btn1,					// メニュー
	Max
};

static InputID begin(InputID) { return InputID::Up; }
static InputID end(InputID) { return InputID::Max; }
static InputID operator++(InputID& state) { return (state = static_cast<InputID>(std::underlying_type<InputID>::type(state) + 1)); }
static InputID operator*(const InputID& state) { return state; }
