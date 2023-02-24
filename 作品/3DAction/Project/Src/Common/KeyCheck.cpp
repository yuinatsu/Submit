#include <DxLib.h>
#include "KeyCheck.h"


//�ϐ��̒�`
//---------------------------
//  true : �L�[��������Ă��� �������� ��Ԃ�����������
bool keyOld[KEY_LIST_MAX];		// �P�t���[���O�̃L�[�̏��
bool keyNew[KEY_LIST_MAX];		// ���t���[���̃L�[�̏��
bool keyTrgDown[KEY_LIST_MAX];	// KeyDown�̃g���K�[���
bool keyTrgUp[KEY_LIST_MAX];	// KeyUp�̃g���K�[���


// �L�[���̏�����
void KeyInit(void)
{
	for (int k = 0; k < KEY_LIST_MAX; k++)
	{
		keyOld[k] = false;
		keyNew[k] = false;
		keyTrgDown[k] = false;
		keyTrgUp[k] = false;
	}
}


// �L�[���̍쐬
void CheckKeyList(void)
{
	// �@�P�t���[���O�̃f�[�^���R�s�[����FkeyOld
	// �@���̑��̕ϐ��͏�����
	for (int k = 0; k < KEY_LIST_MAX; k++)
	{
		keyOld[k] = keyNew[k];
		keyNew[k] = false;
		keyTrgDown[k] = false;
		keyTrgUp[k] = false;
	}

	// �A���t���[���̏�Ԃ��擾���i�[�FkeyNew

	//�V�X�e���֘A
	keyNew[KEY_SYS_START] = CheckHitKey(KEY_INPUT_SPACE);

	//Player1�֘A
	keyNew[KEY_P1_UP] = CheckHitKey(KEY_INPUT_UP);
	keyNew[KEY_P1_DOWN] = CheckHitKey(KEY_INPUT_DOWN);
	keyNew[KEY_P1_LEFT] = CheckHitKey(KEY_INPUT_LEFT);
	keyNew[KEY_P1_RIGHT] = CheckHitKey(KEY_INPUT_RIGHT);
	keyNew[KEY_P1_A] = CheckHitKey(KEY_INPUT_N);
	keyNew[KEY_P1_B] = CheckHitKey(KEY_INPUT_M);

	//Player2�֘A
	keyNew[KEY_P2_UP] = CheckHitKey(KEY_INPUT_W);
	keyNew[KEY_P2_DOWN] = CheckHitKey(KEY_INPUT_S);
	keyNew[KEY_P2_LEFT] = CheckHitKey(KEY_INPUT_A);
	keyNew[KEY_P2_RIGHT] = CheckHitKey(KEY_INPUT_D);
	keyNew[KEY_P2_A] = CheckHitKey(KEY_INPUT_LCONTROL);
	keyNew[KEY_P2_B] = CheckHitKey(KEY_INPUT_LSHIFT);

	// �B�g���K�[�̏�Ԃ��`�F�b�N���i�[�FkeyTrgDown�AkeyTrgUp

	for (int k = 0; k < KEY_LIST_MAX; k++)
	{
		keyTrgDown[k] = keyNew[k] && !keyOld[k];
		keyTrgUp[k] = !keyNew[k] && keyOld[k];
	}

}

