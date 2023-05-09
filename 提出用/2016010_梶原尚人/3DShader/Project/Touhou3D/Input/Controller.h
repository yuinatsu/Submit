#pragma once
#include <map>
#include <array>
#include <string>
#include "InputID.h"
#include "../Common/Vector2.h"

// �g���K�[���
enum class Trg
{
	Now,				// ����
	Old,				// ��O
	Max
};

// ���͑��u�̎��
enum class CntType
{
	Key,				// �L�[�{�[�h
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
	// �X�V
	virtual void Update(double delta) = 0;
	// �R���g���[���[�̎�ނ̎擾
	virtual CntType GetCntType(void) = 0;

	// �������Ă��邩
	bool Press(InputID id);
	// �������u��
	bool Pressed(InputID id);
	// �������u��
	bool Released(InputID id);
	// �ړ����͗L��
	const Vector2& GetLeftInput(void) const&
	{
		return leftInput_;
	}
protected:
	// ���͏��
	inputData data_;
	// �ړ��p�̓��͂��ꂽ����
	Vector2 leftInput_;
};

