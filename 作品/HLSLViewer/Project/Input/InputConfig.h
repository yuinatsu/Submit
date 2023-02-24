#pragma once
#include <map>
#include <string>
#include "InputID.h"

#define lpConfigMng (InputConfig::GetInstance())

using InputCodeTbl = std::map<std::string, int>;
using InputCode = std::map<InputID, int>;

class InputConfig
{
public:
	static void Create(void);
	static void Destroy(void);
	static InputConfig& GetInstance(void);

	// �L�[�{�[�h�p�̃R�[�h���擾
	const InputCode& GetKeyInputCode(void);
	// �L�[�{�[�h�p�̃R�[�h�̎Q��
	void SetKeyInputCode(InputID id, int code);
	// �L�[�{�[�h�̃R�[�h�����ׂăf�t�H���g�Ƃ���
	void SetDefalutKeyCode(void);
	// �ݒ肵�Ă���L�[�R�[�h��ύX(�L�[�{�[�h�p)
	void SwapKeyInputCode(InputID id, int code);
private:
	InputConfig();
	~InputConfig();
	InputConfig(const InputConfig&) = delete;
	void operator=(const InputConfig&) = delete;
	// �Q�[���J�n���Ƀ��[�h�����
	bool Load(const std::string& fname, InputCode& in);
	// �Q�[���I�����Ɍ��ݎg���Ă���L�[�R�[�h�ɕύX�����(�Z�[�u�����)
	bool Save(const std::string& fname, InputCode& in);
	static InputConfig* sInstance_;
	InputCode keyInputCode_;
	InputCode joypadInputCode_;
	InputCode joypadInputMove_;
};

