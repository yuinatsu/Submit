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

//using TrgBool = std::array<bool, static_cast<size_t>(Trg::Max)>;	// ���͏�		std::array<���͐^�U, �g���K�[���>
//using CntData = std::map<InputID, TrgBool>;							// ���͏��		std::map<�L�[���, TrgBool>
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

	/// <summary> ������ </summary>
	/// <returns> ������true���s��false </returns>
	virtual bool Init(void) = 0;

	/// <summary> �X�V </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	virtual void Update(double delta) = 0;

	/// <summary> �R���g���[���[�̎�ނ̎擾 </summary>
	/// <returns> �R���g���[���[�̎�� </returns>
	virtual CntType GetCntType(void) = 0;

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

	/// <summary> �������Ă��邩 </summary>
	/// <returns> ������true���s��false </returns>
	bool MovePress(bool isMove);

	/// <summary> �������Ȃ��Ȃ����� </summary>
	/// <returns> ������true���s��false </returns>
	bool MoveReleased();

	/// <summary> �������n�߂��� </summary>
	/// <returns> ������true���s��false </returns>
	bool MovePressed();

	/// <summary> �������Ă��邩 </summary>
	/// <returns> ������true���s��false </returns>
	bool WalkPress();

	/// <summary> ������������Ă��Ȃ��� </summary>
	/// <returns> ������true���s��false </returns>
	bool IsNeutral();

	/// <summary> ���������ꂽ�� </summary>
	/// <returns> ������true���s��false </returns>
	bool IsAnyPress();

	/// <summary> ������ </summary>
	/// <param name="id"> �L�[�̎�� </param>
	/// <param name="limit"> �������̍ő厞�� </param>
	/// <param name="delta"> �f���^�^�C�� </param>
	/// <returns> ������true���s��false </returns>
	std::string LongPress(InputID id, double limit, double delta);
	const inputData& Get(void);
private:
protected:
	/// <summary> ���͏�� </summary>
	//CntData cntData_;
	inputData data_;
	/// <summary> �L�[�̃��X�g��� </summary>
	//std::map<InputID, unsigned int> keyList_ = {};

	/// <summary> LongPress�֌W </summary>
	bool isPress_;
	bool isFinish_;
	float PressCount_;
};

