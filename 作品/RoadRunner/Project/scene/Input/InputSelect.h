#pragma once
#include <DxLib.h>
#include <memory>
#include "Joypad.h"
#include "Keyboard.h"


struct InputSelect
{
	std::unique_ptr <Input> operator()()
	{
		// ����@��I��
		//---------------
		std::unique_ptr <Input> input_;
		if (GetJoypadNum())									// �R���g���[���[
		{
			input_ = std::make_unique<Joypad>();			// �W���C�p�b�h���ڑ�����Ă�����Joypad�N���X
		}
		else												// �L�[�{�[�h
		{
			input_ = std::make_unique<Keyboard>();			// keyboard�N���X
		}
		return std::move(input_);
	}
};
