#pragma once
#include <map>
#include <array>
#include <string>
#include "InputID.h"
#include "../Vector2.h"
#include "../../SceneManager.h"

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

	// ������ 
	virtual bool Init(void) = 0;

	// �X�V 
	virtual void Update(float delta) = 0;

	// �R���g���[���[�̎�ނ̎擾 
	virtual CntType GetCntType(void) = 0;

	// �����N���b�N���Ă��邩 
	//bool MousePress(InputID id);

	// �������Ă��邩 
	bool Press(InputID id);

	// �������u�� 
	bool Pressed(InputID id);

	// �������u�� 
	bool Released(InputID id);

	// ��������Ă��Ȃ��Ƃ� 
	bool NotPress(InputID id);

	// ������������Ă��Ȃ��� 
	bool IsNeutral();

	// �J�[�\���ʒu���Z�b�g(�f�t�H���g�����͒��S) 
	virtual void SetCursorPos(const Vector2& pos = lpSceneMng.screenSize_<float> / 2.0f) = 0;

	// �J�[�\���ʒu�̎擾 
	const Vector2& GetCursorPos(void) const&
	{
		return cursorPos_;
	}

	// �J�[�\���̃X�s�[�h���Z�b�g 
	void SetCursorSpeed(float speed)
	{
		cursorSpeed_ = speed;
	}

	// ��(�ړ�)�p 
	const Vector2& GetLeftInput(void) const&
	{
		return leftInput_;
	}

	// ���_�ړ��Ɏg�����W���擾 
	const Vector2& GetRightInput(void) const&
	{
		return rightInput_;
	}

	// ����{�^����������Ă��邩 
	bool PressDecision(void) const;

	// ����{�^���������ꂽ�u�Ԃ� 
	bool PressedDecision(void) const;

	// �L�����Z���{�^���������ꂽ�� 
	bool PressCancel(void) const;

	// �L�����Z���{�^���������ꂽ�u�Ԃ� 
	bool PressdCancel(void) const;
protected:
	// ���͏�� 
	InputData data_;

	// ���艺���̏��̏��
	std::pair<bool, bool> decisionData_;

	// �L�����Z�����������̃f�[�^
	std::pair<bool, bool> cancelData_;

	// �J�[�\���̍��W
	Vector2 cursorPos_;

	// �J�[�\���̃X�s�[�h(ui���Ɏg�p)
	float cursorSpeed_;

	// �ړ��p�̓��͂��ꂽ����
	Vector2 leftInput_;

	// �U������p���͍��W
	Vector2 rightInput_;
};

