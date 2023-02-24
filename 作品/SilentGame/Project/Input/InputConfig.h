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
	// �L�[�{�[�h�p�̃R�[�h�̎Q��
	const InputCode& GetKeyInputCode(void);
	void SetKeyInputCode(InputID id, int code);

	// �p�b�h�p�̃R�[�h�擾
	// �p�b�h�p�̃R�[�h�̎Q��
	const InputCode& GetJoypadInputCode(void);
	void SetJoypadInputCode(InputID id, int code);
	// �ړ��p(�p�b�h�ł̈ړ�)
	const InputCode& GetJoypadInputMove(void);

	// �p�b�h�R�[�h�����ׂăf�t�H���g���̂ɂ���
	void SetDefalutPadCode(void);

	// �L�[�{�[�h�̃R�[�h�����ׂăf�t�H���g�Ƃ���
	void SetDefalutKeyCode(void);

	// �ݒ肵�Ă���L�[�R�[�h��ύX(�L�[�{�[�h�p)
	void SwapKeyInputCode(InputID id, int code);
	// �ݒ肵�Ă���L�[�R�[�h��ύX(�p�b�h�p)
	void SwapPadInputCode(InputID id, int code);
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

