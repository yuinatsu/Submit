#pragma once
#include "Vector2.h"
//--------------------------------------------------------------------------
// FileSize�F�摜�t�@�C�����̃T�C�Y�����i�[
//--------------------------------------------------------------------------

#define lpSize FileSize::GetInstance()

class FileSize
{
public:
	static FileSize& GetInstance(void)
	{
		static FileSize s_Instance;
		return s_Instance;
	}

	//�y�摜�t�@�C���z
	const Vector2 blockImageSize_{ 288,32 };//�u���b�N�S��
	const Vector2 blockSize_{ 32,32 };//�u���b�N�P��
	const Vector2 blockDiv_{ 9, 1 };//�u���b�N������
	const Vector2 cursorSize_{ 48,48 };//�J�[�\��

	const Vector2 titleLogoSize_{ 700,175 };//�^�C�g���V�[���^�C�g�����S
	const Vector2 titleKeySize_{ 359,103 };//�^�C�g���V�[���L�[���S

	const Vector2 selectLogoSize_{ 600,65 };//�Z���N�g�V�[���Z���N�g���S

	const Vector2 callImageSize_{ 300,240 };//�J�E���g�_�E���S��
	const Vector2 callDiv_{ 1,3 };//�J�E���g�_�E��������
	const Vector2 ruleSize_{ 600,217 };//���[��

	const Vector2 menuImageSize_{ 350,240 };//���j���[���S�S��
	const Vector2 menuSize_{ 350,60 };//���j���[���S�P��
	const Vector2 menuDiv_{ 1,4 };//���j���[���S������

	const Vector2 clearLogoSize_{ 500,80 };//�N���A�V�[���N���A���S

private:
	FileSize() {};
	~FileSize() {};
};
