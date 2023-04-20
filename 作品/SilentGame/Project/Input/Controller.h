#pragma once
#include <map>
#include <array>
#include <string>
#include "InputID.h"

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
	Pad,				// �p�b�h
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
	virtual ~Controller();

	// ������
	virtual bool Init(void) = 0;

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

	// ��������Ă��Ȃ��Ƃ� 
	bool NotPress(InputID id);

	// �������Ă��邩 
	bool MovePress(bool isMove);

	// �������Ȃ��Ȃ����� 
	bool MoveReleased();

	// �������n�߂��� 
	bool MovePressed();

	// �������Ă��邩 
	bool WalkPress();

	// ������������Ă��Ȃ��� 
	bool IsNeutral();

	// ���������ꂽ�� 
	bool IsAnyPress();

	const inputData& Get(void);
private:
protected:
	// ���͏�� 
	inputData data_;
};

