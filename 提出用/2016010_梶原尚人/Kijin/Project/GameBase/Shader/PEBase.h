#pragma once
#include <memory>
#include "PEID.h"
#include "ShrinkBuff.h"
#include "../Common/Vector2.h"

// �d���̐�
struct WEIGHT_NUM
{
	float weight[8];
};

// �K�E�X�A���u���̏d��
constexpr int NUM_WEIGHTS = 8;

class PEBase
{
public:
	PEBase(int postPS, Vector2 pos,Vector2 rate);
	~PEBase();
	virtual void Update(float delta) = 0;
	virtual int Draw(int beforeScr, int afterScr,int depth,int skyScr, int redScr) = 0;
	virtual void SetBuffer(int buff);
	virtual PEID GetPEID(void) = 0;
private:
	// �K�E�X�p�̃V�F�[�_
	int gaussPS_;

protected:
	/// <summary>
	/// �|�X�g�G�t�F�N�g��������ꍇ
	/// </summary>
	/// <param name="firstScr">�|�X�g�G�t�F�N�g�����������X�N���[��</param>
	/// <param name="secondScr">�ڂ����摜</param>
	/// <param name="thirdScr">�[�x�摜</param>
	/// <param name="postPS">�s�N�Z���V�F�[�_</param>
	/// <param name="pos">����̍��W</param>
	/// <param name="rate">�X�N���[���̔{��</param>
	void SetPostEffect(int firstScr, int secondScr, int thirdScr,int depthScr, int postPS, Vector2 pos, Vector2 rate);

	// �ǂ̃|�X�g�G�t�F�N�g�������Ȃ��ꍇ
	void DefaultScreen(int berforeScr,int afterScr, Vector2 pos, Vector2 rate);
	/// <summary>
	/// �K�E�X�A���u���p
	/// </summary>
	/// <param name="pos">����̍��W</param>
	/// <param name="scr">�X�N���[��</param>
	/// <param name="div">�󂯎�����X�N���[�������H���邩�ǂ���</param>
	/// <param name="vs">�c�����̒��_�V�F�[�_</param>
	void SetXYBlurEffect(Vector2 pos, int scr, Vector2 div, int vs);

	/// <summary>
	/// �d�݌v�Z
	/// </summary>
	/// <param name="Weights"></param>
	/// <param name="sigma"></param>
	void WeitghtsGaussian(int Weights, float sigma);

	// �������g��Draw
	int ownScr_;

	// �c�u���[�p�̃X�N���[��
	int vertBlur_;
	// ���u���[�p�̃X�N���[��
	int sideBlur_;

	// �d�݌v�Z
	WEIGHT_NUM* weight;
	WEIGHT_NUM weights_;
	int weibuff_;

	// �X�N���[���̃T�C�Y
	Vector2* screen;
	Vector2 screen_;
	int scrbuff_;

	// ����̍��W
	Vector2 pos_;
	// �摜�̕`��{��
	Vector2 rate_;
	// �|�X�g�G�t�F�N�g�p�̃V�F�[�_
	int postPS_;

	// �o�b�t�@�󂯎��
	int buff_;

	// �k���o�b�t�@�쐬�p�N���X
	std::shared_ptr<ShrinkBuff> shrink_;
};

