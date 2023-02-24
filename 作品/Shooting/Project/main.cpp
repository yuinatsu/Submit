//-------------------------------
// �Q�[������	�FProject1
// ����			�F2016010 �������l
//-------------------------------

#include "DxLib.h"			// DX���C�u�����̎g�p
#include "time.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"

// �萔��`
//------------------
#define SCREEN_SIZE_X 640		// ��ʂ̉��T�C�Y
#define SCREEN_SIZE_Y 480		// ��ʂ̏c�T�C�Y
#define BG_SIZE_Y	960			// �w�i�̏c�T�C�Y
#define PLAYER_SIZE_X 32		// ���@�̉��T�C�Y
#define PLAYER_SIZE_Y 32		// ���@�̏c�T�C�Y
#define SHOT_SIZE_X 32			// ���@�̒e�̉��T�C�Y
#define SHOT_SIZE_Y 32			// ���@�̒e�̏c�T�C�Y
#define ENEMY_SIZE_X 48			// �G�̉��T�C�Y
#define ENEMY_SIZE_Y 48			// �G�̏c�T�C�Y

// �ϐ���`
//------------------
// �w�i�֌W
//------------------
int bgImage;					// �O���t�B�b�N�n���h��
int bgPosY1;					// �w�i�P��Y���W
int bgPosY2;					// �w�i�Q��Y���W
int bgSpeed;					// �ړ���

// �v���C���[�֘A
//------------------
int playerImage;				// �O���t�B�b�N�n���h��
int playerPosX;					// X���W
int playerPosY;					// Y���W
int playerSpeed;				// �ړ���
int count;						// ���G���ԃJ�E���g
bool playerAlive;				// �����t���O

// �e�֘A
//------------------
int shotImage;					// �O���t�B�b�N�n���h��
int shotPosX;					// X���W
int shotPosY;					// Y���W
int shotSpeed;					// �ړ���
bool shotAlive;					// ���˃t���O
double shotRad;					// �p�x

// �G�֘A
//------------------
int enemyImage;					// �O���t�B�b�N�n���h��
int enemyPosX;					// X���W
int enemyPosY;					// Y���W
int enemySpeed;					// �ړ���
bool enemyAlive;				// �����t���O

// WinMain�֐�
//------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �V�X�e������
	//-------------------
	SetWindowText("�V���[�e�B���O");						// �Q�[���E�B���h�E�̃^�C�g��
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);			// �Q�[���E�B���h�E�̃T�C�Y�ƐF���[�h��ݒ�
	ChangeWindowMode(true);									// �Q�[���E�B���h�E�̕\�����@ (true : window     false : �t���X�N���[��)
	if (DxLib_Init() == -1)									// DX���C�u�����̏���������		Init:�C�j�V�����C�Y
	{
		TRACE("DxLib �̏��������s");
		return -1;											// DX���C�u�����̏��������s�̈׃V�X�e���I��
	}

	srand((unsigned int)time(NULL));

	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);

	// �O���t�B�b�N�̓o�^		�Q�[���������O�ɁA�V�X�e����������ɂ��Ȃ��Ƃ����Ȃ�(DxLib_Init�̒�`���K�v�Ȃ���)
	//--------------------
	bgImage = LoadGraph("image/haikei.png");
	if (bgImage == -1)
	{
		TRACE("�w�i�摜�̓ǂݍ��݂Ɏ��s���܂���");
	}
	playerImage = LoadGraph("image/player.png");
	if (playerImage == -1)
	{
		TRACE("���@�摜�̓ǂݍ��݂Ɏ��s���܂���");
	}

	shotImage = LoadGraph("image/shot.png");
	if (shotImage == -1)
	{
		TRACE("�e�摜�̓ǂݍ��݂Ɏ��s���܂���");
	}

	enemyImage = LoadGraph("image/enemy2.png");
	if (enemyImage == -1)
	{
		TRACE("�G�摜�̓ǂݍ��݂Ɏ��s���܂���");
	}

	// �ϐ��̏�����
	//--------------------
	bgPosY1 = 0;					// �w�i�P�̏����ʒu
	bgPosY2 = -BG_SIZE_Y;			// �w�i�Q�̏����ʒu
	bgSpeed = 2;

	playerPosX	= (SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;
	playerPosY	= SCREEN_SIZE_Y - PLAYER_SIZE_Y;
	playerSpeed = 3;
	count = 0;
	playerAlive = true;

	shotPosX  = 0;
	shotPosY  = 0;
	shotSpeed = 6;
	shotAlive = false;

	enemyPosX  = (SCREEN_SIZE_X - ENEMY_SIZE_X) / 2;
	enemyPosY  = -ENEMY_SIZE_Y;
	enemySpeed = 3;
	enemyAlive = false;

	// �Q�[�����[�v
	//--------------------
	while ((ProcessMessage() == 0) && (CheckHitKey(KEY_INPUT_ESCAPE) == 0))
	{
		_dbgStartDraw();
		// �Q�[���̃��C������
		//---------------------

		// �w�i�ړ�����
		//---------------------
		bgPosY1 += bgSpeed;
		if (bgPosY1 >= SCREEN_SIZE_Y)
		{
			bgPosY1 = -(BG_SIZE_Y + SCREEN_SIZE_Y);			// bgPosY1 = bgPosY2 - BG_SIZE_Y;
		}
		bgPosY2 += bgSpeed;
		if (bgPosY2 >= SCREEN_SIZE_Y)
		{
			bgPosY2 = -(BG_SIZE_Y + SCREEN_SIZE_Y);			// bgPosY2 = bgPosY1 - BG_SIZE_Y;
		}

		// ���@�ړ�����
		//---------------------

// �����蔻��@�E�����ŎQ�Ƃ�����W�͂��ׂĂ��P�t���[���O

		if (!playerAlive)					// if(playerAlive == false)
		{
			playerPosX = (SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;
			playerPosY = SCREEN_SIZE_Y - PLAYER_SIZE_Y;
			playerAlive = true;
		}
		else
		{
			if (CheckHitKey(KEY_INPUT_RIGHT) == 1)
			{
				if (playerPosX + playerSpeed <= SCREEN_SIZE_X - PLAYER_SIZE_X)
				{
					playerPosX += playerSpeed;					// �E�ړ�
				}
				else
				{
					playerPosX = SCREEN_SIZE_X - PLAYER_SIZE_X;		// �E�ړ�����
				}
			}
			if (CheckHitKey(KEY_INPUT_LEFT) == 1)
			{
				if (playerPosX - playerSpeed >= 0)
				{
					playerPosX -= playerSpeed;					// ���ړ�
				}
				else
				{
					playerPosX = 0;									// ���ړ�����
				}
			}
			if (CheckHitKey(KEY_INPUT_UP) == 1)
			{
				if (playerPosY - playerSpeed >= 0)
				{
					playerPosY -= playerSpeed;					// ��ړ�
				}
				else
				{
					playerPosY = 0;									// ��ړ�����
				}
			}
			if (CheckHitKey(KEY_INPUT_DOWN) == 1)
			{
				if (playerPosY + playerSpeed <= SCREEN_SIZE_Y - PLAYER_SIZE_Y)
				{
					playerPosY += playerSpeed;					// ���ړ�
				}
				else
				{
					playerPosY = SCREEN_SIZE_Y - PLAYER_SIZE_Y;		// ���ړ�����
				}
			}
		}

// �����蔻��A�E�����ŎQ�Ƃ�����W�͎��@�͌��t���[���A�G�ƒe�͂P�t���[���O

		// �G�ړ�����
		//---------------------
		if (enemyAlive == true)
		{
			enemyPosY += enemySpeed;
			if (enemyPosY >= SCREEN_SIZE_Y)
			{
				enemyAlive = false;
			}
		}
		else
		{
			// �����ŗ~�����͈� : 0 �` (SCREEN_SIZE_X - ENEMY_SIZE_X)
			enemyPosX = rand() % (SCREEN_SIZE_X - ENEMY_SIZE_X);				// ��ʓ��ŗ���
			enemyPosY = -ENEMY_SIZE_Y;
			enemyAlive = true;
		}

		// �e����
		//---------------------
		if (shotAlive == false)					// ���˂��Ă��Ȃ��Ƃ�
		{
			if (CheckHitKey(KEY_INPUT_Z) == 1 && playerAlive == true)
			{
				shotAlive = true;
				shotPosX = playerPosX;
				shotPosY = playerPosY;
			}
		}
		else
		{										// ���˂��Ă���Ƃ�
			shotPosY -= shotSpeed;
			if (shotPosY <= -SHOT_SIZE_Y)		// ��ʊO�ɍs������
			{
				shotAlive = false;
			}
		}

// �����蔻��B�E�����ŎQ�Ƃ�����W�͎��@�A�G�A�e���Ɍ��t���[��

		// �����蔻��
		//---------------------
		if ((shotAlive == true) && (enemyAlive == true))
		{
			if ((shotPosY <= enemyPosY + ENEMY_SIZE_Y) && (shotPosY + SHOT_SIZE_Y >= enemyPosY) && (shotPosX <= enemyPosX + ENEMY_SIZE_X) && (shotPosX + SHOT_SIZE_X >= enemyPosX))
			{
				shotAlive = false;
				enemyAlive = false;
				TRACE("������\n");
			}
		}

		if ((playerAlive == true) && (enemyAlive == true) && count == 0)
		{
			if ((playerPosY <= enemyPosY + ENEMY_SIZE_Y) && (playerPosY + PLAYER_SIZE_Y >= enemyPosY) && (playerPosX <= enemyPosX + ENEMY_SIZE_X) && (playerPosX + PLAYER_SIZE_X >= enemyPosX))
			{
				playerAlive = false;
				enemyAlive = false;
				TRACE("���G�J�n\n");
				count = 450;					// ��O�b��
			}
		}
		if (count > 0)
		{
			count--;
			if (count <= 0)
			{
				TRACE("���G�I��\n");
				count = 0;
				playerAlive = true;
			}
		}

		// �`�揈��
		//---------------------
		SetDrawScreen(DX_SCREEN_BACK);	// �`�悷���ʂ𗠂̉�ʂɐݒ�
		ClsDrawScreen();				// �`�悷���ʂ̓��e������

		DrawGraph(0, bgPosY1, bgImage, false);					// �N���A�O������ʃR�s�[���
		DrawGraph(0, bgPosY2, bgImage, false);
		if (enemyAlive == true)
		{
			DrawGraph(enemyPosX, enemyPosY, enemyImage, true);
		}
		if (shotAlive == true)
		{
			DrawGraph(shotPosX, shotPosY, shotImage, true);
		}
		if (playerAlive == true)
		{
			DrawGraph(playerPosX, playerPosY, playerImage, true);
		}

		_dbgAddDraw();
		ScreenFlip();					// ���̉�ʂ�\�̉�ʂɏu�ԃR�s�[
	}

	// �V�X�e���I������
	//--------------------
	DxLib::DxLib_End();			// DX���C�u�����̏I������
	return 0;			// �Q�[���̏I��
}