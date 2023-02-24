//---------------------------------
// �Q�[������F�C���x�[�_�[
// �����@�@�@�F2016010 �������l
//---------------------------------

#include "DxLib.h"			// DX���C�u�����̎g�p
#include "time.h"
#include "main.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

// �ϐ���`
//-----------------
int x, y;											// �z����W
int blinkCnt;										// �_�ŕ\���J�E���^

// �V�[���֌W
//-----------------
SCENE_ID scnID;										// ���݂̃V�[��

// �L�[�o�b�t�@
//-----------------
char keyBuf[256];									// ���݂̃L�[�o�b�t�@
char keyBufOld[256];								// 1�t���[���O�̃L�[�o�b�t�@

// �^�C�g���֌W
//-----------------
int titleImage;										// �O���t�B�b�N�n���h��
int titlePosX;										// X���W
int titlePosY;										// Y���W

// �Q�[���I�[�o�[�֌W
//-----------------
int gameOverImage;									// �O���t�B�b�N�n���h��
int gameOverPosX;									// X���W
int gameOverPosY;									// Y���W
int gameOverCnt;									// ���Y���W

// �X�R�A�֌W
//-----------------
int scrData;										// ���݂̃X�R�A						�̂̓X�R�A��500�Ƃ�����A��2����00�͉�ʕ\���݂̂ɂ��āA�������ɂ�5�݂̂��i�[���邱�ƂŃ������̐ߖ���s����
int hiScrData;										// �n�C�X�R�A

// ���@�֌W
//-----------------
int playerImage[DIR_MAX][ANIM_MAX];					// �O���t�B�b�N�n���h��
int playerPosX;										// X���W
int playerPosY;										// Y���W
int playerSpeed;									// �ړ���
bool playerAlive;									// �����t���O
int playerAnimCnt;									// �A�j���[�V�����t���O
int pAnimCnt;										// �A�j���[�V�����J�E���^
int playerLife;										// ���@�̎c�@��

// �G�֌W
//-----------------
int enemyImage[ENEMY_TYPE_MAX][DIR_MAX][ANIM_MAX];	// �O���t�B�b�N�n���h��
int enemyPosX[ENEMY_COUNT_Y][ENEMY_COUNT_X];		// X���W
int enemyPosY[ENEMY_COUNT_Y][ENEMY_COUNT_X];		// Y���W
DIR_ID enemyDir[ENEMY_COUNT_Y][ENEMY_COUNT_X];		// �G�̔��˕���
int enemySpeed;										// �ړ���
bool enemyAlive[ENEMY_COUNT_Y][ENEMY_COUNT_X];		// �����t���O
int enemyMoveCnt;									// �G�̈ړ��J�E���^
int enemyAnimCnt;									// �A�j���[�V�����t���O
int eAnimCnt;										// �A�j���[�V�����J�E���^
MOVE_LR enemyMoveFlagX;								// �G�̍��E�ړ��Ǘ��p
MOVE_MODE enemyMoveMode;							// �G�̍��E�ړ������ړ��̊Ǘ��p
DIR_ID dir;											// �G�����]���A�j���[�V����

// ���@�̒e�֌W
//-----------------
int pShotImage;										// �O���t�B�b�N�n���h��
int pShotPosX;										// X���W
int pShotPosY;										// Y���W
int pShotSpeed;										// �ړ���
bool pShotAlive;									// ���˃t���O

// �G�̒e�֌W
//-----------------
int eShotImage;										// �O���t�B�b�N�n���h��
int eShotPosX[ENEMY_COUNT_X];						// X���W
int eShotPosY[ENEMY_COUNT_X];						// Y���W
int eShotSpeed;										// �ړ���
bool eShotAlive[ENEMY_COUNT_X];						// ���˃t���O

// WinMain�֐�
//-----------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!SysInit())
	{
		TRACE("���������s");
		return -1;
	}

	// �Q�[�����[�v
	//---------------------
	while (ProcessMessage() == 0 && keyBuf[KEY_INPUT_ESCAPE] == 0)
	{
		memcpy(keyBufOld, keyBuf, sizeof(keyBufOld));			// sizeof(keyBufOld) / sizeof(keyBufOld[0])
		GetHitKeyStateAll(keyBuf);								// ���ׂẴL�[�̉�����Ԃ��擾

		_dbgStartDraw();

		// �V�[���ݒ菈��
		//---------------------
		switch (scnID)							// ����l��case�ł��ׂď���.....�ُ�l�Ɛ���l�����݂����Ă��܂�����
		{
		case SCENE_ID_TITLE:
			TitleScene();				// �^�C�g���V�[���Ɉڍs
			break;

		case SCENE_ID_GAME:
			GameScene();				// �Q�[���V�[���Ɉڍs
			break;

		case SCENE_ID_GAMEOVER:
			GameOverScene();			// �Q�[���I�[�o�[�V�[���Ɉڍs
			break;

		case SCENE_ID_RULE:
			RuleScene();				// ���[�������V�[���Ɉڍs
			break;

		default:
			break;
		}
	}
	
	// �V�X�e���I������
	//--------------------
	DxLib::DxLib_End();		// DX���C�u�����̏I������
	return 0;		// �Q�[���̏I��
}

// �^�C�g�����
//---------------------
void TitleScene(void)
{
	TitleDraw();
	if (keyBuf[KEY_INPUT_Z] && !keyBufOld[KEY_INPUT_Z])
	{
		GameInit();								// �Q�[���V�[���p������
		scnID = SCENE_ID_GAME;					// Z�L�[����������Q�[���V�[���Ɉړ�
	}
}

// �^�C�g���`�揈��
//---------------------
void TitleDraw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	DrawBox(GAME_SCREEN_X, GAME_SCREEN_Y, GAME_SCREEN_X + GAME_SCREEN_SIZE_X, GAME_SCREEN_Y + GAME_SCREEN_SIZE_Y, GetColor(30, 30, 30), true);			// �Q�[���X�N���[��

	DrawGraph(titlePosX, titlePosY, titleImage, true);					// �^�C�g�����S�`��

	if ((blinkCnt / 30) % 2)
	{
		int ScrWidth = GetDrawStringWidth("PHSH Z KEY", strlen("PHSH Z KEY"));
		DrawString(GAME_SCREEN_X + (GAME_SCREEN_SIZE_X - ScrWidth) / 2, GAME_SCREEN_Y + (GAME_SCREEN_SIZE_Y * 2 / 3), "PUSH Z KEY", 0x00ff00);				// "PHSH Z KEY"�̕������`��
	}
	ScrDraw();			// �X�R�A�\��
	ScreenFlip();
	blinkCnt++;			// �_�ŃJ�E���^
}

// ���[�������p���
//---------------------
void RuleScene(void)
{
	RuleDraw();

	if (keyBuf[KEY_INPUT_Z] && !keyBufOld[KEY_INPUT_Z])
	{
		scnID = SCENE_ID_RULE;					// Z�L�[���������烋�[���V�[���Ɉړ�
	}
}

// ���[�������\��
//---------------------
void RuleDraw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	DrawBox(GAME_SCREEN_X, GAME_SCREEN_Y, GAME_SCREEN_X + GAME_SCREEN_SIZE_X, GAME_SCREEN_Y + GAME_SCREEN_SIZE_Y, GetColor(30, 30, 30), true);		// �Q�[���X�N���[��

	ScreenFlip();
}

// �Q�[�����
//---------------------
void GameScene(void)
{
	PlayerCtl();			// �v���C���[����
	PshotCtl();				// �v���C���[�̒e����
	EnemyCtl();				// �G����
	CheckHitCtl();			// �����蔻�萧��
	EshotCtl();				// �G�̒e����

	GameDraw();
}

// ���@����
//---------------------
void PlayerCtl(void)
{
	// ���@���A����
	//-------------------------
	if (!playerAlive)					// ���@�����ꂽ�Ƃ�
	{
		playerAlive = true;				// ����
	}
	// ���@�ړ�����
	//---------------------
	if (keyBuf[KEY_INPUT_RIGHT] == 1)
	{
		if (playerPosX + playerSpeed <= GAME_SCREEN_SIZE_X - PLAYER_SIZE_X)
		{
			playerPosX += playerSpeed;					// �E�ړ�
		}
		else
		{
			playerPosX = GAME_SCREEN_SIZE_X - PLAYER_SIZE_X;		// �E�ړ�����
		}
	}
	if (keyBuf[KEY_INPUT_LEFT] == 1)
	{
		if (playerPosX - playerSpeed >= 0)
		{
			playerPosX -= playerSpeed;					// ���ړ�
		}
		else
		{
			playerPosX = 0;											// ���ړ�����
		}
	}
}

// ���@�̒e����
//---------------------
void PshotCtl(void)
{
	if (!pShotAlive)					// ���˂��Ă��Ȃ��Ƃ�
	{
		if ((keyBuf[KEY_INPUT_Z] == 1) && (!keyBufOld[KEY_INPUT_Z]))
		{
			pShotAlive = true;				// ����
			// �e�̏������W
			//-------------
			pShotPosX = playerPosX + (PLAYER_SIZE_X / 2);
			pShotPosY = playerPosY;
		}
	}
	else
	{											// ���˂��Ă���Ƃ�
		pShotPosY -= pShotSpeed;	// �e�ړ�
		if (pShotPosY <= 0)			// ��ʊO�ɍs������
		{
			pShotAlive = false;		// ����
		}
	}
}

// �G����
//------------------
void EnemyCtl(void)
{
	if (enemyAlive)
	{
		// �G�ړ�����
		//---------------------
		if (enemyMoveMode == MOVE_MODE_DOWN)
		{
			// �G���ړ�����
			//---------------------
			dir = DIR_DOWN;										// �G�������]���A�j���[�V����
			if (!(enemyMoveCnt % ENEMY_MOVE_INT))
			{
				if ((ENEMY_COUNT_Y - 1) - enemyMoveCnt < ENEMY_COUNT_Y)
				{
					// �G�ړ�
					for (x = 0; x < ENEMY_COUNT_X; x++)
					{
						enemyPosY[(ENEMY_COUNT_Y - 1) - (enemyMoveCnt / ENEMY_MOVE_INT)][x] += GET_ENEMY_POS_Y(1);
						enemyDir[(ENEMY_COUNT_Y - 1) - (enemyMoveCnt / ENEMY_MOVE_INT)][x] = DIR_DOWN;
					}
				}
			}
		}
		else
		{
			// �G���E�ړ�����
			//---------------------
			if (!(enemyMoveCnt % ENEMY_MOVE_INT))
			{
				if ((ENEMY_COUNT_X - 1) - enemyMoveCnt < ENEMY_COUNT_X)
				{
					for (y = 0; y < ENEMY_COUNT_Y; y++)
					{
						if (enemyMoveCnt % 5 == 0)
						{
							if (enemyMoveFlagX == MOVE_RIGHT)
							{
								// �E�ړ�
								dir = DIR_RIGHT;						// �G�E�����]���A�j���[�V����
								enemyPosX[y][(ENEMY_COUNT_X - 1) - ((enemyMoveCnt / ENEMY_MOVE_INT) % ENEMY_COUNT_X)] += enemySpeed;
								enemyDir[y][(ENEMY_COUNT_X - 1) - ((enemyMoveCnt / ENEMY_MOVE_INT) % ENEMY_COUNT_X)] = DIR_RIGHT;
							}
							else
							{
								// ���ړ�
								dir = DIR_LEFT;							// �G�������]���A�j���[�V����
								enemyPosX[y][(enemyMoveCnt / ENEMY_MOVE_INT) % ENEMY_COUNT_X] -= enemySpeed;
								enemyDir[y][(enemyMoveCnt / ENEMY_MOVE_INT) % ENEMY_COUNT_X] = DIR_LEFT;
							}
						}
					}
					// �G�̒e���ˏ���
					//---------------------
					for (x = 0; x < ENEMY_COUNT_X; x++)
					{
						if (!eShotAlive[x])
						{
							// ���m���Ŕ���
							if ((rand() % 300) == 0)
							{
								for (y = ENEMY_COUNT_Y - 1; y >= 0; y--)
								{
									if (enemyAlive[y][x])
									{
										eShotPosX[x] = enemyPosX[y][x];		// X���W������
										eShotPosY[x] = enemyPosY[y][x];		// Y���W������
										eShotAlive[x] = true;				// ����
										enemyDir[y][x] = DIR_DOWN;			// ���ˎ��͉�������
										break;
									}
								}
							}
						}
					}
				}
			}
		}
		enemyMoveCnt++;

		// �G�������i�K�ړ�
		//---------------------
		if (enemyMoveMode == MOVE_MODE_DOWN)
		{
			if (enemyMoveCnt / ENEMY_MOVE_INT >= ENEMY_COUNT_Y)
			{
				enemyMoveCnt = 0;
				enemyMoveMode = MOVE_MODE_LR;		// ���E�ړ��ɖ߂�
				CheckEnemyLine();			// �G�̖h�q���C�����B����
			}
		}
		else
		{
			CheckEnemyMoveLR();				// �G�����]���m�F
		}
	}
}

// �G�̖h�q���C�����B�m�F
//---------------------
void CheckEnemyLine(void)
{
	// �G�̍ŉ��i�ʒu�̃T�[�`
	for (y = ENEMY_COUNT_Y - 1; y >= 0; y--)
	{
		for (x = 0; x < ENEMY_COUNT_X; x++)
		{
			if (enemyAlive[y][x])
			{
				goto END;		// �ŉ��i���m�F������END���x���̈ʒu�ɐ�����ڂ�
			}
		}
	}
	END:

	// �T�[�`�����ŉ��i�̓G���h�q���C���ɓ��B���Ă��邩���`�F�b�N����
	if (enemyPosY[y][x] >= GET_ENEMY_POS_Y(ENEMY_COUNT_Y + 3))
	{
		// ���@���S����
		if (DethProcPlayer())
		{
			for (y = 0; y < ENEMY_COUNT_Y; y++)
			{
				for (x = 0; x < ENEMY_COUNT_X; x++)
				{
					enemyPosY[y][x] -= GET_ENEMY_POS_Y(3);			// �G���W��������
				}
			}
		}
	}
}

// ���@�̎��S������
//---------------------
bool DethProcPlayer(void)
{
	playerAlive = false;			// ���@����
	if (playerLife > 0)				// �c�@���c���Ă�Ƃ�
	{
		playerLife--;			// �c�@�����炷
		return true;
	}
	// �c�@���Ȃ��Ȃ����Ƃ��iGAME OVER�j
	scnID = SCENE_ID_GAMEOVER;			// �Q�[���I�[�o�[��ʂֈړ�
	return false;
}

// �G�����]���m�F						...�G�ړ��ƈꏏ�ɏ������Ȃ�
//---------------------
void CheckEnemyMoveLR(void)
{
	// �G���E�����i�K�ړ�
	//---------------------
	if (enemyMoveCnt % (5 * ENEMY_COUNT_X) != 0)
	{
		return;
	}

	for (y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (x = 0; x < ENEMY_COUNT_X; x++)
		{
			if (enemyMoveFlagX == MOVE_RIGHT)
			{
				if (enemyPosX[y][x] >= GAME_SCREEN_SIZE_X - ENEMY_SIZE_X)		// �E�[
				{
					enemyMoveFlagX = MOVE_LEFT;			// ���͍��Ɉړ�
					enemyMoveMode = MOVE_MODE_DOWN;		// �ړ����������ɐݒ�
					enemyMoveCnt = 0;
					return;
				}
			}
			else
			{
				if (enemyPosX[y][x] <= 0)									// ���[
				{
					enemyMoveFlagX = MOVE_RIGHT;			// ���͉E�Ɉړ�
					enemyMoveMode = MOVE_MODE_DOWN;		// �ړ����������ɐݒ�
					enemyMoveCnt = 0;
					return;
				}
			}
		}
	}
}

// �G�̒e����
//---------------------
void EshotCtl(void)
{
	for (x = 0; x < ENEMY_COUNT_X; x++)
	{
		if (!eShotAlive[x])					// ���˂��Ă��Ȃ��Ƃ�
		{
			continue;
		}
		else								// ���˂��Ă���Ƃ�
		{
			eShotPosY[x] += pShotSpeed;		// �e�ړ�
			if (eShotPosY[x] >= GAME_SCREEN_SIZE_Y - ENEMY_SHOT_SIZE_Y)
			{
				eShotAlive[x] = false;		// ��ʊO�ɏo�������
			}
		}
	}
}

// �����蔻�菈��
//---------------------
void CheckHitCtl(void)
{
	CheckHitPshotEnemy();			// ���@�e�ƓG�̔���
	CheckHitEshotPlayer();			// �G�e�Ǝ��@�̔���
}

// ���@�̒e�ƓG�̓����蔻�菈��
//---------------------
void CheckHitPshotEnemy(void)
{
	if (pShotAlive)
	{
		for (y = 0; y < ENEMY_COUNT_Y; y++)				// �s�F��	��F�c
		{
			for (x = 0; x < ENEMY_COUNT_X; x++)
			{
				if (enemyAlive[y][x])			// �G�������Ă���Ώ���
				{
					if ((pShotPosY <= enemyPosY[y][x] + ENEMY_SIZE_Y) && 
						(pShotPosY + PLAYER_SHOT_SIZE_Y >= enemyPosY[y][x]) && 
						(pShotPosX <= enemyPosX[y][x] + ENEMY_SIZE_X) && 
						(pShotPosX + PLAYER_SHOT_SIZE_X >= enemyPosX[y][x]))		// �����蔻��
					{
						// �G��|�������̏���
						//---------------------
						AddScr(ENEMY_POINT);				// �X�R�A���Z
						pShotAlive = false;					// ���@�e����
						enemyAlive[y][x] = false;			// �G���j
						return;
					}
				}
			}
		}
	}
}

// ���@�ƓG�̓����蔻�菈��
//---------------------
void CheckHitEshotPlayer(void)
{
	for (int n = 0; n < ENEMY_COUNT_X; n++)
	{
		if (eShotAlive[n])
		{
			if (playerAlive)			// ���@�������Ă���Ώ���
			{
				if ((eShotPosY[n] <= playerPosY + PLAYER_SIZE_Y) && 
					(eShotPosY[n] + ENEMY_SHOT_SIZE_Y >= playerPosY) && 
					(eShotPosX[n] <= playerPosX + PLAYER_SIZE_X) && 
					(eShotPosX[n] + ENEMY_SHOT_SIZE_X >= playerPosX))			// �����蔻��
				{
					// �����������̏���
					//---------------------
					eShotAlive[n] = false;		// �G�e����
					DethProcPlayer();			// ���@���S����
					return;
				}
			}
		}
	}
}

// �E�X�R�A���\������
//---------------------
void ScrDraw(void)
{
	DrawCount(2, "SCORE", scrData, "HI SCORE");			// ����̃X�R�A�f�[�^
	DrawCount(6, "HI-SCORE", hiScrData, "HI-SCORE");	// �n�C�X�R�A�f�[�^
}

// �X�R�A�\������
//---------------------
void DrawCount(int rows, const char* str, int Cnt, const char* lenStr)
{
	DrawString(UI_DRAW_X, GAME_SCREEN_Y + GAME_FONT_SIZE * rows, str, 0xffffff);						// �X�R�A�\��
	int ScrWidth = GetDrawStringWidth(lenStr, strlen(lenStr));						// SCORE�̕����̒������擾
	int ScrCntWidth = GetDrawFormatStringWidth("%d", Cnt);							// scrData�̐��l�̒������擾
	DrawFormatString(UI_DRAW_X + ScrWidth - ScrCntWidth, GAME_SCREEN_Y + GAME_FONT_SIZE * (rows + 1), 0xffffff, "%d", Cnt);					// �X�R�A�����\��			���W�ɋ߂����̂��瑫���Ă���
}

// �X�R�A���Z����
//---------------------
bool AddScr(int num)
{
	scrData += num;					// �X�R�A���Z
	if (scrData > hiScrData)				// ���݂̃X�R�A���n�C�X�R�A�𒴂����ꍇ
	{
		hiScrData = scrData;		// �n�C�X�R�A�X�V
	}
	return true;
}

// �Q�[���`�揈��
//---------------------
void GameDraw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);	// �`�悷���ʂ𗠂̉�ʂɐݒ�
	ClearDrawScreen();				// �`�悷���ʂ̓��e������

	DrawBox(GAME_SCREEN_X, GAME_SCREEN_Y, GAME_SCREEN_X + GAME_SCREEN_SIZE_X, GAME_SCREEN_Y + GAME_SCREEN_SIZE_Y, GetColor(30, 30, 30), true);			// �Q�[���X�N���[��

	DrawLine(GAME_SCREEN_X, GAME_SCREEN_Y + GET_ENEMY_POS_Y(ENEMY_COUNT_Y + 3), GAME_SCREEN_X + GAME_SCREEN_SIZE_X, GAME_SCREEN_Y + GET_ENEMY_POS_Y(ENEMY_COUNT_Y + 3), 0xffffff);			// �{�[�_�[���C��

	for (int n = 0; n < playerLife; n++)
	{
		DrawGraph(UI_DRAW_X + PLAYER_SIZE_X * n, GAME_SCREEN_Y + GAME_SCREEN_SIZE_Y - PLAYER_SIZE_Y, playerImage[DIR_DOWN][1], true);					// �c�@�\��
	}

	eAnimCnt = abs(((enemyAnimCnt / 10) % 4) - 2);						// 0.1.2.1.0.1.2.1.0........
	if ((eAnimCnt >= 0) && (eAnimCnt < ANIM_MAX))
	{
		for (y = 0; y < ENEMY_COUNT_Y; y++)				// �s�F��	��F�c
		{
			for (x = 0; x < ENEMY_COUNT_X; x++)
			{
				// �������G�\��
				if (enemyAlive[y][x] == true)
				{
					DrawGraph(GAME_SCREEN_X + enemyPosX[y][x], GAME_SCREEN_Y + enemyPosY[y][x], enemyImage[y % (ENEMY_TYPE_MAX - 1)][enemyDir[y][x]][eAnimCnt], true);
				}
			}
		}
	}

	if (pShotAlive)
	{
		// ���������@�e�\��
		DrawGraph(GAME_SCREEN_X + pShotPosX, GAME_SCREEN_Y + pShotPosY, pShotImage, true);
	}

	pAnimCnt = abs(((playerAnimCnt / 15) % 4) - 2);							// 0.1.2.1.0.1.2.1.0........
	if ((pAnimCnt >= 0) && (pAnimCnt < ANIM_MAX))
	{
		// ���@�\��
		DrawGraph(GAME_SCREEN_X + playerPosX, GAME_SCREEN_Y + playerPosY, playerImage[DIR_UP][pAnimCnt], true);
	}

	for (x = 0; x < ENEMY_COUNT_X; x++)
	{
		if (eShotAlive[x])					// ���@���D��x����
		{
			// �G�̒e�\��
			DrawGraph(GAME_SCREEN_X + eShotPosX[x], GAME_SCREEN_Y + eShotPosY[x], eShotImage, true);
		}
	}

	ScrDraw();			// �X�R�A�\��

	_dbgAddDraw();
	ScreenFlip();			// ���̉�ʂ�\�̉�ʂɏu�ԃR�s�[

	// �A�j���[�V�����J�E���g
	enemyAnimCnt++;
	playerAnimCnt++;
}

// �Q�[���I�[�o�[���
//---------------------
void GameOverScene(void)
{
	GameOverDraw();

	// �J�[�\���ړ�
	if (keyBuf[KEY_INPUT_UP] && !keyBufOld[KEY_INPUT_UP])
	{
		gameOverCnt = 2;
	}
	if (keyBuf[KEY_INPUT_DOWN] && !keyBufOld[KEY_INPUT_DOWN])
	{
		gameOverCnt = 4;
	}

	// ���g���C
	if (gameOverCnt == 2)
	{
		if (keyBuf[KEY_INPUT_Z] && !keyBufOld[KEY_INPUT_Z])
		{
			GameInit();
			scnID = SCENE_ID_GAME;
		}
	}
	// �^�C�g���Ɉړ�
	if (gameOverCnt == 4)
	{
		if (keyBuf[KEY_INPUT_Z] && !keyBufOld[KEY_INPUT_Z])
		{
			scnID = SCENE_ID_TITLE;
		}
	}
}


// �Q�[���I�[�o�[�`�揈��
//---------------------
void GameOverDraw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	DrawBox(GAME_SCREEN_X, GAME_SCREEN_Y, GAME_SCREEN_X + GAME_SCREEN_SIZE_X, GAME_SCREEN_Y + GAME_SCREEN_SIZE_Y, GetColor(30, 30, 30), true);			// �Q�[���X�N���[��

	DrawGraph(gameOverPosX, gameOverPosY, gameOverImage, true);					// �Q�[���I�[�o�[���S�`��

	int ScrWidth = GetDrawStringWidth("CONTINUE?", strlen("CONTINUE?"));
	DrawString(GAME_SCREEN_X + (GAME_SCREEN_SIZE_X - ScrWidth) / 2, GAME_SCREEN_Y + (GAME_SCREEN_SIZE_Y / 2) + GAME_FONT_SIZE, "CONTINUE?",0xff0000);					// "CONTINUE?"�̕������`��

	ScrWidth = GetDrawStringWidth("YES", strlen("YES"));
	DrawString(GAME_SCREEN_X + (GAME_SCREEN_SIZE_X - ScrWidth) / 2, GAME_SCREEN_Y + (GAME_SCREEN_SIZE_Y * 2 / 3) + GAME_FONT_SIZE * 2, "YES", 0x00ff00);				// "YES"�̕������`��
	ScrWidth = GetDrawStringWidth("NO", strlen("NO"));
	DrawString(GAME_SCREEN_X + (GAME_SCREEN_SIZE_X - ScrWidth) / 2, GAME_SCREEN_Y + (GAME_SCREEN_SIZE_Y * 2 / 3) + GAME_FONT_SIZE * 4, "NO", 0x00ff00);					// "NO"�̕������`��

	if ((blinkCnt / 30) % 2)
	{
		ScrWidth = GetDrawStringWidth("��", strlen("��"));
		DrawString(GAME_SCREEN_X + (GAME_SCREEN_SIZE_X - ScrWidth) / 2 - 30, GAME_SCREEN_Y + (GAME_SCREEN_SIZE_Y * 2 / 3) + GAME_FONT_SIZE * gameOverCnt, "��", 0x00ff00);			// "��"�̕������`��

	}
	ScrDraw();			// �X�R�A�\��
	ScreenFlip();
	blinkCnt++;
}

// �V�X�e���n����
//---------------------
bool SysInit(void)
{
	// �V�X�e������
	//-------------------
	SetWindowText("�C���x�[�_�[");	// �Q�[���E�B���h�E�̃^�C�g��
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);			// �Q�[���E�B���h�E�̃T�C�Y�ƐF���[�h��ݒ�
	ChangeWindowMode(true);				// �Q�[���E�B���h�E�̕\�����@ (true : window     false : �t���X�N���[��)
	if (DxLib_Init() == -1)				// DX���C�u�����̏���������		Init:�C�j�V�����C�Y
	{
		TRACE("DxLib �̏��������s");
		return false;						// DX���C�u�����̏��������s�̈׃V�X�e���I��
	}

	srand((unsigned int)time(NULL));

	memset(keyBuf, 0, sizeof(keyBuf));					// keyBuf���ׂĂɂO���Z�b�g		�v�f�� sizeof(keyBuf) * �o�C�g�� sizeof(keyBuf[0])
	memset(keyBufOld, 0, sizeof(keyBufOld));

	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);

	// �O���t�B�b�N�̓o�^
	//--------------------
	const char* enemyFileName[ENEMY_TYPE_MAX] = {			// �G�̎�ރf�[�^
		"image/Enemy 15-1.png",
		"image/Enemy 15-2.png",
		"image/Enemy 15-3.png",
		"image/Enemy 15-4.png",
		"image/Enemy 15-5.png",
		"image/Enemy 15-6.png",
		"image/Enemy 09-1.png"
	};

	titleImage = LoadGraph("image/TitleLogo2.png");				// �^�C�g���摜�ǂݍ���
	if (titleImage == -1)
	{
		TRACE("�^�C�g���摜�̓ǂݍ��݂Ɏ��s���܂���")
	}

	gameOverImage = LoadGraph("image/GameOver.png");		// �Q�[���I�[�o�[�摜�ǂݍ���
	if (gameOverImage == -1)
	{
		TRACE("�Q�[���I�[�o�[�摜�̓ǂݍ��݂Ɏ��s���܂���")
	}

	// �G�̎�ޕ��ǂݍ���
	for (int n = 0; n < ENEMY_TYPE_MAX; n++)
	{
		// �G�̃A�j���[�V���������ǂݍ���
		if (LoadDivGraph(enemyFileName[n], ANIM_MAX * DIR_MAX, ANIM_MAX, DIR_MAX, ENEMY_SIZE_X, ENEMY_SIZE_Y, &enemyImage[n][0][0]) == -1)
		{
			return false;
		}
	}

	pShotImage = LoadGraph("image/pshot.png");				// ���@�̒e�ǂݍ���
	if (pShotImage == -1)
	{
		TRACE("�e�摜�̓ǂݍ��݂Ɏ��s���܂���");
	}
											// �l�n��																// �|�C���^�[�n��
	if (LoadDivGraph("image/Male 05-1.png", ANIM_MAX * DIR_MAX, ANIM_MAX, DIR_MAX, PLAYER_SIZE_X, PLAYER_SIZE_Y, &playerImage[0][0]) == -1)		// ���@�ǂݍ���
	{
		return false;
	}

	eShotImage = LoadGraph("image/eshot.png");				// �G�̒e�\��
	if (eShotImage == -1)
	{
		TRACE("�e�摜�̓ǂݍ��݂Ɏ��s���܂���");
	}

	// �ϐ��̏�����
	//--------------------
	scnID = SCENE_ID_TITLE;							// �����V�[���ݒ�

	scrData = 0;						// �����X�R�A�f�[�^
	hiScrData = 2000;					// �n�C�X�R�A�f�[�^

	blinkCnt = 0;						// �_�ŕ\���J�E���^
	SetFontSize (GAME_FONT_SIZE);		// �t�H���g�T�C�Y�ݒ�

	titlePosX = GAME_SCREEN_X + ((GAME_SCREEN_SIZE_X - TITLE_SIZE_X) / 2);		// �^�C�g���摜X���W������
	titlePosY = GAME_SCREEN_Y + ((GAME_SCREEN_SIZE_Y - TITLE_SIZE_Y) / 3);		// �^�C�g���摜Y���W������

	gameOverPosX = GAME_SCREEN_X + ((GAME_SCREEN_SIZE_X - GAMEOVER_SIZE_X) / 2);		// �Q�[���I�[�o�[�摜X���W������
	gameOverPosY = GAME_SCREEN_Y + ((GAME_SCREEN_SIZE_Y - GAMEOVER_SIZE_Y) / 4);		// �Q�[���I�[�o�[�摜Y���W������

	return true;
}

// �Q�[���V�X�e���n����
//---------------------
bool GameInit(void)
{
	// �ϐ��̏�����
	//--------------------
	scrData = 0;						// �����X�R�A�f�[�^

	gameOverCnt = 2;					// �Q�[���I�[�o�[���̃J�[�\���ʒu

	playerPosX = (GAME_SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;		// ���@X���W
	playerPosY = GAME_SCREEN_SIZE_Y - PLAYER_SIZE_Y;		// ���@Y���W
	playerAlive = true;						// ���@�����t���O
	playerSpeed = 4;						// ���@�̈ړ��X�s�[�h
	playerAnimCnt = 0;					// ���@�̃A�j���[�V�����J�E���^
	playerLife = 2;						// ���݂̎��@�Ƃ͕ʂ�2�� => 3��

	for (y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (x = 0; x < ENEMY_COUNT_X; x++)
		{
			enemyPosX[y][x] = (x * ENEMY_SIZE_X) * 3 / 2;		// �GX���W
			enemyPosY[y][x] = GET_ENEMY_POS_Y(y + 2);		// �GY���W
			enemyAlive[y][x] = true;			// �G�����t���O
			enemyDir[y][x] = DIR_DOWN;			// �G�̏����ړ�����
		}
	}
	enemySpeed = 17;						// �G�̈ړ��X�s�[�h
	enemyMoveCnt = 0;						// �G�̈ړ��J�E���^
	enemyAnimCnt = 0;						// �G�̃A�j���[�V�����J�E���^
	enemyMoveFlagX = MOVE_RIGHT;			// �G�̍��E�ړ��Ǘ�
	enemyMoveMode = MOVE_MODE_LR;			// �ړ��������@���Eor��

	pShotPosX = 0;		// ���@�̒eX���W
	pShotPosY = 0;		// ���@�̒eY���W
	pShotAlive = false;	// ���@�̒e�����t���O
	pShotSpeed = 6;		// ���@�̒e�̈ړ��X�s�[�h

	for (x = 0; x < ENEMY_COUNT_X; x++)
	{
		eShotPosX[x] = 0;		// �G�̒eX���W
		eShotPosY[x] = 0;		// �G�̒eY���W
		eShotAlive[x] = false;	// �G�̒e�����t���O
	}
	eShotSpeed = 3;				// �G�̒e�̈ړ��X�s�[�h

	return true;
}
