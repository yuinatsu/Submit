#pragma once
#include <memory>
#include <list>
#include <vector>
#include <array>
#include "PEID.h"
#include "../Common/Vector2.h"

class PEBase;

class PEManager
{
public:
	PEManager();
	~PEManager();
	void Update(float delta);
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="beforeScr">�|�X�g�G�t�F�N�g�����������X�N���[��</param>
	/// <param name="afterScr">�`�悷��X�N���[��</param>
	/// <param name="depth">�[�x�l�摜</param>
	/// <param name="skyScr">�X�J�C�h�[���݂̂̉摜</param>
	/// <param name="redScr">�X�J�C�h�[���ƃX�e�[�W�̉摜</param>
	void Draw(int beforeScr,int afterScr,int depth, int skyScr,int redScr);
	/// <summary>
	/// �o�b�t�@���󂯎��
	/// </summary>
	/// <param name="buff">�o�b�t�@</param>
	void SetBuffer(int buff);
	/// <summary>
	/// �ǂ̃|�X�g�G�t�F�N�g���I���ɂ��邩
	/// </summary>
	/// <param name="id">�|�X�g�G�t�F�N�g��id</param>
	/// <param name="flag">true�ɂ��邩false�ɂ��邩</param>	
	void SetFlag(PEID id,bool flag = true);
	/// <summary>
	/// �w�肵��id�̃|�X�g�G�t�F�N�g�̃t���O���ǂ��������擾����
	/// </summary>
	/// <param name="id">�|�X�g�G�t�F�N�g��id</param>
	/// <returns></returns>
	bool GetFlag(PEID id);
	/// <summary>
	/// �Ή�����|�X�g�G�t�F�N�g���g�p����(�f�o�b�O���̂�)
	/// </summary>
	/// <param name="id">�ǂ̃|�X�g�G�t�F�N�g�Ȃ̂�</param>
	/// <param name="hitkey">�ǂ̃L�[�Ȃ̂�</param>
	void CheckPostEffect(PEID id, int hitkey);
private:
	// �g�p����V�F�[�_���i�[����ϐ�
	std::array<int, 6> postPS_;
	// ����̍��W
	Vector2 pos_ = {0.0f,0.0f};
	// �`�悷��T�C�Y�̔{��
	Vector2 rate_ = { 1.0f,1.0f };
	// �|�X�g�G�t�F�N�g���܂Ƃ߂ĊǗ�����
	std::array<
		std::pair<std::shared_ptr<PEBase>, bool>
		, 7> PostEffect_;
	// �Ή������L�[�������Ă��邩�ǂ���
	std::array<bool,
		static_cast<size_t>(PEID::Max) + 1> usePE_;
	// �V�F�[�_�����炩���߃Z�b�g���Ă���
	void ShaderLoad(void);
	// PostEffect_�̃t���O�����ׂ�false�ɂ���
	void FlagSetFalse(void);
};

