#pragma once
#include <map>
#include <unordered_map>
#include <string>
#include "../InputID.h"
#include "../../../Shader/PEID.h"

#define lpConfigMng (InputConfig::GetInstance())

using InputCodeTbl = std::map<std::string, int>;
using InputCode = std::map<InputID, int>;

class InputConfig
{
	using TypeDataTbl = std::unordered_map<int, std::pair<std::string, void(InputConfig::*)(void)>>;
	using PeConfig = std::unordered_map<PEID, bool>;
public:
	static void Create(void);
	static void Destroy(void);
	static InputConfig& GetInstance(void);


	/// <summary>
	/// �w���ID�̃R�[�h��ύX����(����Ă���ꍇ�͓���ւ��)
	/// </summary>
	/// <param name="id"> �ύX������ID </param>
	/// <param name="code"> �ύX����l </param>
	void ChangeInputCode(InputID id, int code);

	/// <summary>
	/// �f�t�H���g�̐ݒ�ɖ߂�
	/// </summary>
	/// <param name=""></param>
	void SetDefalutCode(void);

	

	/// <summary>
	/// �ݒ肳�ꂽcontroller�Ŏg�p����R�[�h���擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> �R�[�h </returns>
	const InputCode& GetInputCode(void) const&
	{
		return inputCode_;
	}

	void SetInputCode(InputCode& code)
	{
		inputCode_ = code;
	}

	/// <summary>
	/// ���݂̓��̓^�C�v��������
	/// </summary>
	/// <param name=""></param>
	/// <returns> ���݂̓��̓^�C�v(int) </returns>
	const int GetNowType(void) const
	{
		return nowType_;
	}

	/// <summary>
	/// �|�X�g�G�t�F�N�g�̃R���t�B�O�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const PeConfig& GetPeConfig(void) const&;

	/// <summary>
	/// �|�X�g�G�t�F�N�g�̃t���O���Z�b�g����
	/// </summary>
	/// <param name="id"></param>
	/// <param name="flag"></param>
	void SetPeConfig(PEID id, bool flag);

	/// <summary>
	/// �J�����̃X�s�[�h���Z�b�g����
	/// </summary>
	/// <param name="speed"></param>
	void SetCameraSpeed(float speed)
	{
		camSpeed_ = speed;
	}

	/// <summary>
	/// �J�����̃X�s�[�h���擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	float GetCameraSpeed(void) const
	{
		return camSpeed_;
	}
private:
	InputConfig();
	void InitSound();
	void InitInput();
	~InputConfig();
	InputConfig(const InputConfig&) = delete;
	void operator=(const InputConfig&) = delete;

	/// <summary>
	/// �v���C�X�e�[�V�����n�p�̃f�t�H���g�ɖ߂�����
	/// </summary>
	/// <param name=""></param>
	void SetPsDefalutCode(void);

	/// <summary>
	/// Xbox�n�p�̃f�t�H���g�ɖ߂鏈��
	/// </summary>
	/// <param name=""></param>
	void SetXboxDefalutCode(void);

	/// <summary>
	/// �L�[�{�[�h�p�̃f�t�H���g�ɖ߂�����
	/// </summary>
	/// <param name=""></param>
	void SetKeyDefalutCode(void);

	// �Q�[���J�n���Ƀ��[�h�����
	bool Load(const std::string& fname);

	// �Q�[���I�����Ɍ��ݎg���Ă���L�[�R�[�h�ɕύX�����(�Z�[�u�����)
	bool Save(const std::string& fname);
	static InputConfig* sInstance_;

	// �R�[�h
	InputCode inputCode_;

	// ���݂̓��̓^�C�v
	int nowType_;

	// �|�X�g�G�t�F�N�g�p�R���t�B�O
	PeConfig peConfig_;

	// �J�����̊��x
	float camSpeed_;

	// �^�C�v�ʂɐݒ�t�@�C�����ƃf�t�H���g�ɖ߂��֐����܂Ƃ߂��e�[�u��
	static TypeDataTbl typeDataTbl_;
};

