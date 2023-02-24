#pragma once
#include <map>
#include <string>

// ���͏��
using InputData = std::map<std::string, int>;

// ���̓^�C�v
enum class InputType
{
	NOW,	// ���݂̓��͏��
	OLD		// �ߋ��̓��͏��
};

class Obj;

class Input
{
public:
	Input();
	virtual ~Input();
	virtual bool Update(void) = 0;				// �X�V
	const InputData& GetData(InputType type);	// ���̓f�[�^�擾
private:
protected:
	InputData data_;							// ���݂̓���
	InputData dataOld_;							// �O�̓���
};
