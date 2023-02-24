#pragma once
#include <type_traits>

// キーデータ
enum class InputID {
	Up,					// 上
	Down,				// 下
	Left,				// 左
	Right,				// 右
	Jump,				// ジャンプ
	Btn1,				// 決定
	Btn2,				// キャンセル
	Btn3,				// 色変え欄表示
	Btn4,				// ポーズ
	Escape,
	Neutral,
	Max
};

static InputID Converter(const std::string str) {
	if (str == "up")
	{ 
		return InputID::Up; 
	}
	else if (str == "down")
	{
		return InputID::Down; 
	}
	else if (str == "left") 
	{
		return InputID::Left;
	}
	else if (str == "right")
	{
		return InputID::Right;
	}
	else if (str == "jump") 
	{ 
		return InputID::Jump; 
	}
	else if (str == "btn1")
	{
		return InputID::Btn1; 
	}
	else if (str == "btn2")
	{
		return InputID::Btn2;
	}
	else if (str == "btn3")
	{
		return InputID::Btn3; 
	}
	else if (str == "btn4")
	{
		return InputID::Btn4;
	}
	else if (str == "escape")
	{
		return InputID::Escape; 
	}
	else if (str == "neutral") 
	{
		return InputID::Neutral;
	}
	return InputID::Max;
};

static std::string Converter(const InputID id) {
	if (id == InputID::Up)
	{
		return "up";
	}
	else if (id == InputID::Down)
	{
		return "down";
	}
	else if (id == InputID::Left)
	{ 
		return "left"; 
	}
	else if (id == InputID::Right)
	{
		return "right";
	}
	else if (id == InputID::Jump)
	{
		return "jump"; 
	}
	else if (id == InputID::Btn1) 
	{
		return "btn1";
	}
	else if (id == InputID::Btn2)
	{
		return "btn2"; 
	}
	else if (id == InputID::Btn3) 
	{
		return "btn3";
	}
	else if (id == InputID::Btn4)
	{
		return "btn4";
	}
	else if (id == InputID::Escape)
	{
		return "escape";
	}
	else if (id == InputID::Neutral) 
	{
		return "neutral";
	}
	return "none";
}

static InputID begin(InputID) { return InputID::Up; };
static InputID end(InputID) { return InputID::Max; };
static InputID operator++(InputID& num) { return num = InputID(std::underlying_type<InputID>::type(num) + 1); };
static InputID operator*(const InputID& state) { return state; };

