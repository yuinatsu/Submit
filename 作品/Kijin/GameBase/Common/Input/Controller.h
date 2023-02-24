#pragma once
#include <map>
#include <array>
#include <string>
#include "InputID.h"
#include "../Vector2.h"
#include "../../Application.h"

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

//using TrgBool = std::array<bool, static_cast<size_t>(Trg::Max)>;	// ���͏�		std::array<���͐^�U, �g���K�[���>
//using CntData = std::map<InputID, TrgBool>;							// ���͏��		std::map<�L�[���, TrgBool>
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

	/// <summary> ������ </summary>
	/// <returns> ������true���s��false </returns>
	virtual bool Init(void) = 0;

	/// <summary> �X�V </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	virtual void Update(float delta) = 0;

	/// <summary> �R���g���[���[�̎�ނ̎擾 </summary>
	/// <returns> �R���g���[���[�̎�� </returns>
	virtual CntType GetCntType(void) = 0;

	/// <summary> �����N���b�N���Ă��邩 </summary>
	/// <param name="id">�L�[�̎��</param>
	/// <returns>��������true���s����false</returns>
	bool MousePress(InputID id);

	/// <summary> �������Ă��邩 </summary>
	/// <param name="id"> �L�[�̎�� </param>
	/// <returns> ������true���s��false </returns>
	bool Press(InputID id);

	/// <summary> �������u�� </summary>
	/// <param name="id"> �L�[�̎�� </param>
	/// <returns> ������true���s��false </returns>
	bool Pressed(InputID id);

	/// <summary> �������u�� </summary>
	/// <param name="id"> �L�[�̎�� </param>
	/// <returns> ������true���s��false </returns>
	bool Released(InputID id);

	/// <summary> ��������Ă��Ȃ��Ƃ� </summary>
	/// <param name="id"> �L�[�̎�� </param>
	/// <returns> ������true���s��false </returns>
	bool NotPress(InputID id);

	/// <summary> ������������Ă��Ȃ��� </summary>
	/// <returns> ������true���s��false </returns>
	bool IsNeutral();

	/// <summary> ���������ꂽ�� </summary>
	/// <returns> ������true���s��false </returns>
	bool IsAnyPress();

	/// <summary>
	/// �J�[�\���ʒu���Z�b�g(�f�t�H���g�����͒��S)
	/// </summary>
	/// <param name="pos"> �J�[�\���̈ʒu </param>
	virtual void SetCursorPos(const Vector2& pos = lpApp.screenSize_<float> / 2.0f) = 0;

	/// <summary>
	/// �J�[�\���ʒu�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> �J�[�\���̍��W </returns>
	const Vector2& GetCursorPos(void) const&
	{
		return cursorPos_;
	}


	/// <summary>
	/// �J�[�\���̃X�s�[�h���Z�b�g
	/// </summary>
	/// <param name="speed"> �X�s�[�h </param>
	void SetCursorSpeed(float speed)
	{
		cursorSpeed_ = speed;
	}

	const float GetCursorSpeed(void) const
	{
		return cursorSpeed_;
	}

	const Vector2& GetInputVec(void) const&
	{
		return inputVec_;
	}

	/// <summary> ������ </summary>
	/// <param name="id"> �L�[�̎�� </param>
	/// <param name="limit"> �������̍ő厞�� </param>
	/// <param name="delta"> �f���^�^�C�� </param>
	/// <returns> ������true���s��false </returns>
	std::string LongPress(InputID id, double limit, double delta);
	const InputData& Get(void);
private:
protected:
	/// <summary> ���͏�� </summary>
	//CntData cntData_;
	InputData data_;

	/// <summary> �L�[�̃��X�g��� </summary>
	//std::map<InputID, unsigned int> keyList_ = {};

	// �J�[�\���̍��W
	Vector2 cursorPos_;

	// �J�[�\���̃X�s�[�h(pad�����g���)
	float cursorSpeed_;

	// �ړ��p�̓��͂��ꂽ����
	Vector2 inputVec_;

	/// <summary> LongPress�֌W </summary>
	bool isPress_;
	bool isFinish_;
	float PressCount_;
};

