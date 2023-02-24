#pragma once
#include <array>
#include <map>
#include <vector>
#include <string>
#include "inputID.h"

// �g���K�[���
enum class Trg 
{
	Now,			// ����
	Old,			// 1�O
	Max
};

// �R���g�[���[���[�^�C�v
enum class CntType 
{
	Key,			// �L�[�{�[�h
	Pad,			// �p�b�h
	Max
};

using TrgBool = std::array<bool, static_cast<size_t>(Trg::Max)>;
using CntData = std::map<InputID,TrgBool>;

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
	virtual ~Controller();
	virtual bool Init(void) = 0;									// ������
	virtual void Update(double delta) = 0;							// �X�V Update(deltaTime)
	virtual CntType GetCntType(void) = 0;							// �R���g���[������@��̎擾
	const CntData& GetCntData(void) { return cntData_; };			// �ݒ肵��map���i�[���Ă�cntData_�̎擾
	bool Press(InputID id);											// �������Ă��邩
	bool Pressed(InputID id);										// �������u��
	bool Released(InputID id);										// �������u��
	bool SpPress(InputID id);										// ����̃{�^�������������Ă��邩�ǂ���
private:
protected:
	CntData cntData_;
	std::map<InputID, unsigned int> keyList_ = {};
};