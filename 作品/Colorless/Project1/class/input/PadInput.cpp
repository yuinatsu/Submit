#include "PadInput.h"

int PadInput::count_ = 1;

PadInput::PadInput()
{
	Init();
}

PadInput::~PadInput()
{
}

bool PadInput::Init(void)
{
	keyList_.try_emplace(InputID::Up, 0);
	keyList_.try_emplace(InputID::Down, 0);
	keyList_.try_emplace(InputID::Left, 0);
	keyList_.try_emplace(InputID::Right, 0);
	keyList_.try_emplace(InputID::Btn1, XINPUT_BUTTON_X);						// 決定
	keyList_.try_emplace(InputID::Btn2, XINPUT_BUTTON_B);						// キャンセル
	keyList_.try_emplace(InputID::Btn3, XINPUT_BUTTON_RIGHT_SHOULDER);			// 色変え欄表示
	keyList_.try_emplace(InputID::Btn4, XINPUT_BUTTON_RIGHT_THUMB);				// ポーズ
	keyList_.try_emplace(InputID::Escape, XINPUT_BUTTON_BACK);
	keyList_.try_emplace(InputID::Jump,XINPUT_BUTTON_A);						// ジャンプ

	for (auto id : InputID()) 
	{
		cntData_.try_emplace(id, TrgBool{ 0, 0 });
	}
	padID_ = count_++;
	return true;
}


void PadInput::Update(double delta)
{
	GetJoypadXInputState(padID_, &input_);
	// ボタンの更新
	for (auto id : InputID())
	{
		cntData_[id][static_cast<int>(Trg::Old)] = cntData_[id][static_cast<int>(Trg::Now)];
		cntData_[id][static_cast<int>(Trg::Now)] = input_.Buttons[keyList_[id]];
	}
	// スティックの更新
	StickUpdate();
}

void PadInput::StickUpdate(void)
{
	// 左スティックのX軸
	if (input_.ThumbLX > 30000)
	{
		cntData_[InputID::Right][static_cast<int>(Trg::Now)] = 1;
	}
	else if (input_.ThumbLX < -30000)
	{
		cntData_[InputID::Left][static_cast<int>(Trg::Now)] = 1;
	}
	else
	{
		cntData_[InputID::Left][static_cast<int>(Trg::Now)] = 0;
		cntData_[InputID::Right][static_cast<int>(Trg::Now)] = 0;
	}
	// 左スティックのY軸
	if (input_.ThumbLY > 30000)
	{
		cntData_[InputID::Up][static_cast<int>(Trg::Now)] = 1;
	}
	else if (input_.ThumbLY < -30000)
	{
		cntData_[InputID::Down][static_cast<int>(Trg::Now)] = 1;
	}
	else
	{
		cntData_[InputID::Down][static_cast<int>(Trg::Now)] = 0;
		cntData_[InputID::Up][static_cast<int>(Trg::Now)] = 0;
	}
}
