#pragma once
#include "MapCtl.h"
#include "CharImage.h"
#include "Vector2.h"
#include "common.h"

#define PLAYER_MAX 2					// �v���C���[����
#define PLAYER_DEF_SPEED 2              // �v���C���[�ړ���

class MapCtl;							// MapCtl�̃N���X�̃v���g�^�C�v�錾				�����b�g�F�w�b�_��ǂݍ��ނ��R���p�C�����ԒZ�k�A�w�b�_�̏z�Q�Ƃ�h�~

// �L�����̏��
//---------------
enum class PL_ST
{
	NON,			// �L�����I��
	ALIVE,			// ����
	DEAD,			// ���S
	MAX
};

class Player
{
public:
	Player();																		// Player�̐���
	~Player();																		// Player�̔j��
	PL_ST Update(MapCtl& mapCtl, char* keyData, char* keyDataOld);					// Player�̍X�V				// GetKeyState(�}�b�v�f�[�^, ���݂̃L�[�f�[�^, 1�t���[���O�̃L�[�f�[�^)
	void GetKeyState(char* keyData);												// �L�[�f�[�^�擾			// GetkeyState(�L�[�f�[�^)
	void Draw(void);																// Player�̕`��
	bool Init(CHAR_ID charID);														// Player�̏�����			// Init(�I���L����ID)
	bool Relese(void);																// Player�̉��

	CHAR_ID charID_;				// �L�����̎�ޕ�
	PL_ST state_;					// �L�����̏��
	Vector2 pos_;					// ���W

	int keyNow_[static_cast<int>(DIR::MAX)];			// ���͏�Ԃ̃L�[
	int keyOld_[static_cast<int>(DIR::MAX)];			// ���͏�Ԃ̃L�[(1�t���[���O)
private:
	static int pCnt;				// �v���C���[�̑���
	int id_;						// �v���C���[ID		1P:0, 2P:1
	DIR dir_;						// �L�����̌���
	int speed_;						// �ړ���
	CharImage charImage_;			// �L�����摜
	Vector2 IconPos_;				// �L�����A�C�R���̍��W
	int pAnimCnt_;					// �v���C���[�A�j���[�V�����J�E���^

	int keyList_[static_cast<int>(DIR::MAX)];			// �e�����̑Ή��L�[���X�g
};

