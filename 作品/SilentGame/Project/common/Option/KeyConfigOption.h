#pragma once
#include <functional>
#include <array>
#include "../../Input/InputID.h"
#include "OptionWindow.h"



enum class SettingType
{
	Dash,				// ダッシュトリガー
	Crouch,				// しゃがみトリガー
	ItemLeft,			// アイテム選択左カーソル
	ItemRight,			// アイテム選択右カーソル
	Attack,				// アイテム使用、攻撃（長押しで溜め攻撃）
	Init,		        // 初期化
	Exit,               // 戻る
	Max,
};

class KeyConfigOption :
	public OptionWindow<SettingType>
{
public:
	KeyConfigOption(Controller& controller);

private:

	bool Update(float delta) final;
	void Draw(void) final;

	bool UpdateKeyConfig(float delta);
	void DrawKeyConfig(void);
	void DrawPadConfig(void);
	void DrawKeyBoardConfig(void);

	bool UpdateSet(float delta);
	void DrawSet(void);

	// キーボード用の処理
	std::array<char, 256> key_;
	std::array<char, 256> oldkey_;
	// セットするID
	InputID inputID_;
	DINPUT_JOYSTATE state_;
	std::function<bool(float)> updata_;
	std::function <void(void)> draw_;
};

