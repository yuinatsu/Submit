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
	keyList_.try_emplace(InputID::Btn1, XINPUT_BUTTON_X);						// ����
	keyList_.try_emplace(InputID::Btn2, XINPUT_BUTTON_B);						// �L�����Z��
	keyList_.try_emplace(InputID::Btn3, XINPUT_BUTTON_RIGHT_SHOULDER);			// �F�ς����\��
	keyList_.try_emplace(InputID::Btn4, XINPUT_BUTTON_RIGHT_THUMB);				// �|�[�Y
	keyList_.try_emplace(InputID::Escape, XINPUT_BUTTON_BACK);
	keyList_.try_emplace(InputID::Jump,XINPUT_BUTTON_A);						// �W�����v

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
	// �{�^���̍X�V
	for (auto id : InputID())
	{
		cntData_[id][static_cast<int>(Trg::Old)] = cntData_[id][static_cast<int>(Trg::Now)];
		cntData_[id][static_cast<int>(Trg::Now)] = input_.Buttons[keyList_[id]];
	}
	// �X�e�B�b�N�̍X�V
	StickUpdate();
}

void PadInput::StickUpdate(void)
{
	// ���X�e�B�b�N��X��
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
	// ���X�e�B�b�N��Y��
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
