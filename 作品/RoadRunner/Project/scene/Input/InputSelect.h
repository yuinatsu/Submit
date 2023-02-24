#pragma once
#include <DxLib.h>
#include <memory>
#include "Joypad.h"
#include "Keyboard.h"


struct InputSelect
{
	std::unique_ptr <Input> operator()()
	{
		// 操作機種選択
		//---------------
		std::unique_ptr <Input> input_;
		if (GetJoypadNum())									// コントローラー
		{
			input_ = std::make_unique<Joypad>();			// ジョイパッドが接続されていたらJoypadクラス
		}
		else												// キーボード
		{
			input_ = std::make_unique<Keyboard>();			// keyboardクラス
		}
		return std::move(input_);
	}
};
