//---------------------------------
// ゲーム制作：インベーダー
// 氏名　　　：2016010 梶原尚人
//---------------------------------

#include "DxLib.h"			// DXライブラリの使用
#include "time.h"
#include "main.h"
#include "_debug/_DebugDispOut.h"
#include "_debug/_DebugConOut.h"

// 変数定義
//-----------------
int x, y;											// 配列座標
int blinkCnt;										// 点滅表示カウンタ

// シーン関係
//-----------------
SCENE_ID scnID;										// 現在のシーン

// キーバッファ
//-----------------
char keyBuf[256];									// 現在のキーバッファ
char keyBufOld[256];								// 1フレーム前のキーバッファ

// タイトル関係
//-----------------
int titleImage;										// グラフィックハンドル
int titlePosX;										// X座標
int titlePosY;										// Y座標

// ゲームオーバー関係
//-----------------
int gameOverImage;									// グラフィックハンドル
int gameOverPosX;									// X座標
int gameOverPosY;									// Y座標
int gameOverCnt;									// 矢印Y座標

// スコア関係
//-----------------
int scrData;										// 現在のスコア						昔はスコアは500としたら、下2桁の00は画面表示のみにして、メモリには5のみを格納することでメモリの節約を行った
int hiScrData;										// ハイスコア

// 自機関係
//-----------------
int playerImage[DIR_MAX][ANIM_MAX];					// グラフィックハンドル
int playerPosX;										// X座標
int playerPosY;										// Y座標
int playerSpeed;									// 移動量
bool playerAlive;									// 生存フラグ
int playerAnimCnt;									// アニメーションフラグ
int pAnimCnt;										// アニメーションカウンタ
int playerLife;										// 自機の残機数

// 敵関係
//-----------------
int enemyImage[ENEMY_TYPE_MAX][DIR_MAX][ANIM_MAX];	// グラフィックハンドル
int enemyPosX[ENEMY_COUNT_Y][ENEMY_COUNT_X];		// X座標
int enemyPosY[ENEMY_COUNT_Y][ENEMY_COUNT_X];		// Y座標
DIR_ID enemyDir[ENEMY_COUNT_Y][ENEMY_COUNT_X];		// 敵の発射方向
int enemySpeed;										// 移動量
bool enemyAlive[ENEMY_COUNT_Y][ENEMY_COUNT_X];		// 生存フラグ
int enemyMoveCnt;									// 敵の移動カウンタ
int enemyAnimCnt;									// アニメーションフラグ
int eAnimCnt;										// アニメーションカウンタ
MOVE_LR enemyMoveFlagX;								// 敵の左右移動管理用
MOVE_MODE enemyMoveMode;							// 敵の左右移動か下移動の管理用
DIR_ID dir;											// 敵方向転換アニメーション

// 自機の弾関係
//-----------------
int pShotImage;										// グラフィックハンドル
int pShotPosX;										// X座標
int pShotPosY;										// Y座標
int pShotSpeed;										// 移動量
bool pShotAlive;									// 発射フラグ

// 敵の弾関係
//-----------------
int eShotImage;										// グラフィックハンドル
int eShotPosX[ENEMY_COUNT_X];						// X座標
int eShotPosY[ENEMY_COUNT_X];						// Y座標
int eShotSpeed;										// 移動量
bool eShotAlive[ENEMY_COUNT_X];						// 発射フラグ

// WinMain関数
//-----------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (!SysInit())
	{
		TRACE("初期化失敗");
		return -1;
	}

	// ゲームループ
	//---------------------
	while (ProcessMessage() == 0 && keyBuf[KEY_INPUT_ESCAPE] == 0)
	{
		memcpy(keyBufOld, keyBuf, sizeof(keyBufOld));			// sizeof(keyBufOld) / sizeof(keyBufOld[0])
		GetHitKeyStateAll(keyBuf);								// すべてのキーの押下状態を取得

		_dbgStartDraw();

		// シーン設定処理
		//---------------------
		switch (scnID)							// 正常値はcaseですべて書く.....異常値と正常値を混在させてしまうから
		{
		case SCENE_ID_TITLE:
			TitleScene();				// タイトルシーンに移行
			break;

		case SCENE_ID_GAME:
			GameScene();				// ゲームシーンに移行
			break;

		case SCENE_ID_GAMEOVER:
			GameOverScene();			// ゲームオーバーシーンに移行
			break;

		case SCENE_ID_RULE:
			RuleScene();				// ルール説明シーンに移行
			break;

		default:
			break;
		}
	}
	
	// システム終了処理
	//--------------------
	DxLib::DxLib_End();		// DXライブラリの終了処理
	return 0;		// ゲームの終了
}

// タイトル画面
//---------------------
void TitleScene(void)
{
	TitleDraw();
	if (keyBuf[KEY_INPUT_Z] && !keyBufOld[KEY_INPUT_Z])
	{
		GameInit();								// ゲームシーン用初期化
		scnID = SCENE_ID_GAME;					// Zキーを押したらゲームシーンに移動
	}
}

// タイトル描画処理
//---------------------
void TitleDraw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	DrawBox(GAME_SCREEN_X, GAME_SCREEN_Y, GAME_SCREEN_X + GAME_SCREEN_SIZE_X, GAME_SCREEN_Y + GAME_SCREEN_SIZE_Y, GetColor(30, 30, 30), true);			// ゲームスクリーン

	DrawGraph(titlePosX, titlePosY, titleImage, true);					// タイトルロゴ描画

	if ((blinkCnt / 30) % 2)
	{
		int ScrWidth = GetDrawStringWidth("PHSH Z KEY", strlen("PHSH Z KEY"));
		DrawString(GAME_SCREEN_X + (GAME_SCREEN_SIZE_X - ScrWidth) / 2, GAME_SCREEN_Y + (GAME_SCREEN_SIZE_Y * 2 / 3), "PUSH Z KEY", 0x00ff00);				// "PHSH Z KEY"の文字列を描画
	}
	ScrDraw();			// スコア表示
	ScreenFlip();
	blinkCnt++;			// 点滅カウンタ
}

// ルール説明用画面
//---------------------
void RuleScene(void)
{
	RuleDraw();

	if (keyBuf[KEY_INPUT_Z] && !keyBufOld[KEY_INPUT_Z])
	{
		scnID = SCENE_ID_RULE;					// Zキーを押したらルールシーンに移動
	}
}

// ルール説明表示
//---------------------
void RuleDraw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	DrawBox(GAME_SCREEN_X, GAME_SCREEN_Y, GAME_SCREEN_X + GAME_SCREEN_SIZE_X, GAME_SCREEN_Y + GAME_SCREEN_SIZE_Y, GetColor(30, 30, 30), true);		// ゲームスクリーン

	ScreenFlip();
}

// ゲーム画面
//---------------------
void GameScene(void)
{
	PlayerCtl();			// プレイヤー制御
	PshotCtl();				// プレイヤーの弾制御
	EnemyCtl();				// 敵制御
	CheckHitCtl();			// 当たり判定制御
	EshotCtl();				// 敵の弾制御

	GameDraw();
}

// 自機処理
//---------------------
void PlayerCtl(void)
{
	// 自機復帰処理
	//-------------------------
	if (!playerAlive)					// 自機がやられたとき
	{
		playerAlive = true;				// 復活
	}
	// 自機移動処理
	//---------------------
	if (keyBuf[KEY_INPUT_RIGHT] == 1)
	{
		if (playerPosX + playerSpeed <= GAME_SCREEN_SIZE_X - PLAYER_SIZE_X)
		{
			playerPosX += playerSpeed;					// 右移動
		}
		else
		{
			playerPosX = GAME_SCREEN_SIZE_X - PLAYER_SIZE_X;		// 右移動制限
		}
	}
	if (keyBuf[KEY_INPUT_LEFT] == 1)
	{
		if (playerPosX - playerSpeed >= 0)
		{
			playerPosX -= playerSpeed;					// 左移動
		}
		else
		{
			playerPosX = 0;											// 左移動制限
		}
	}
}

// 自機の弾処理
//---------------------
void PshotCtl(void)
{
	if (!pShotAlive)					// 発射していないとき
	{
		if ((keyBuf[KEY_INPUT_Z] == 1) && (!keyBufOld[KEY_INPUT_Z]))
		{
			pShotAlive = true;				// 発射
			// 弾の初期座標
			//-------------
			pShotPosX = playerPosX + (PLAYER_SIZE_X / 2);
			pShotPosY = playerPosY;
		}
	}
	else
	{											// 発射しているとき
		pShotPosY -= pShotSpeed;	// 弾移動
		if (pShotPosY <= 0)			// 画面外に行ったら
		{
			pShotAlive = false;		// 消滅
		}
	}
}

// 敵処理
//------------------
void EnemyCtl(void)
{
	if (enemyAlive)
	{
		// 敵移動制御
		//---------------------
		if (enemyMoveMode == MOVE_MODE_DOWN)
		{
			// 敵下移動処理
			//---------------------
			dir = DIR_DOWN;										// 敵下方向転換アニメーション
			if (!(enemyMoveCnt % ENEMY_MOVE_INT))
			{
				if ((ENEMY_COUNT_Y - 1) - enemyMoveCnt < ENEMY_COUNT_Y)
				{
					// 敵移動
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
			// 敵左右移動処理
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
								// 右移動
								dir = DIR_RIGHT;						// 敵右方向転換アニメーション
								enemyPosX[y][(ENEMY_COUNT_X - 1) - ((enemyMoveCnt / ENEMY_MOVE_INT) % ENEMY_COUNT_X)] += enemySpeed;
								enemyDir[y][(ENEMY_COUNT_X - 1) - ((enemyMoveCnt / ENEMY_MOVE_INT) % ENEMY_COUNT_X)] = DIR_RIGHT;
							}
							else
							{
								// 左移動
								dir = DIR_LEFT;							// 敵左方向転換アニメーション
								enemyPosX[y][(enemyMoveCnt / ENEMY_MOVE_INT) % ENEMY_COUNT_X] -= enemySpeed;
								enemyDir[y][(enemyMoveCnt / ENEMY_MOVE_INT) % ENEMY_COUNT_X] = DIR_LEFT;
							}
						}
					}
					// 敵の弾発射処理
					//---------------------
					for (x = 0; x < ENEMY_COUNT_X; x++)
					{
						if (!eShotAlive[x])
						{
							// 一定確率で発生
							if ((rand() % 300) == 0)
							{
								for (y = ENEMY_COUNT_Y - 1; y >= 0; y--)
								{
									if (enemyAlive[y][x])
									{
										eShotPosX[x] = enemyPosX[y][x];		// X座標初期化
										eShotPosY[x] = enemyPosY[y][x];		// Y座標初期化
										eShotAlive[x] = true;				// 発射
										enemyDir[y][x] = DIR_DOWN;			// 発射時は下を向く
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

		// 敵下方向段階移動
		//---------------------
		if (enemyMoveMode == MOVE_MODE_DOWN)
		{
			if (enemyMoveCnt / ENEMY_MOVE_INT >= ENEMY_COUNT_Y)
			{
				enemyMoveCnt = 0;
				enemyMoveMode = MOVE_MODE_LR;		// 左右移動に戻る
				CheckEnemyLine();			// 敵の防衛ライン到達判定
			}
		}
		else
		{
			CheckEnemyMoveLR();				// 敵方向転換確認
		}
	}
}

// 敵の防衛ライン到達確認
//---------------------
void CheckEnemyLine(void)
{
	// 敵の最下段位置のサーチ
	for (y = ENEMY_COUNT_Y - 1; y >= 0; y--)
	{
		for (x = 0; x < ENEMY_COUNT_X; x++)
		{
			if (enemyAlive[y][x])
			{
				goto END;		// 最下段を確認したらENDラベルの位置に制御を移す
			}
		}
	}
	END:

	// サーチした最下段の敵が防衛ラインに到達しているかをチェックする
	if (enemyPosY[y][x] >= GET_ENEMY_POS_Y(ENEMY_COUNT_Y + 3))
	{
		// 自機死亡処理
		if (DethProcPlayer())
		{
			for (y = 0; y < ENEMY_COUNT_Y; y++)
			{
				for (x = 0; x < ENEMY_COUNT_X; x++)
				{
					enemyPosY[y][x] -= GET_ENEMY_POS_Y(3);			// 敵座標を下げる
				}
			}
		}
	}
}

// 自機の死亡時処理
//---------------------
bool DethProcPlayer(void)
{
	playerAlive = false;			// 自機消滅
	if (playerLife > 0)				// 残機が残ってるとき
	{
		playerLife--;			// 残機を減らす
		return true;
	}
	// 残機がなくなったとき（GAME OVER）
	scnID = SCENE_ID_GAMEOVER;			// ゲームオーバー画面へ移動
	return false;
}

// 敵方向転換確認						...敵移動と一緒に処理しない
//---------------------
void CheckEnemyMoveLR(void)
{
	// 敵左右方向段階移動
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
				if (enemyPosX[y][x] >= GAME_SCREEN_SIZE_X - ENEMY_SIZE_X)		// 右端
				{
					enemyMoveFlagX = MOVE_LEFT;			// 次は左に移動
					enemyMoveMode = MOVE_MODE_DOWN;		// 移動方向を下に設定
					enemyMoveCnt = 0;
					return;
				}
			}
			else
			{
				if (enemyPosX[y][x] <= 0)									// 左端
				{
					enemyMoveFlagX = MOVE_RIGHT;			// 次は右に移動
					enemyMoveMode = MOVE_MODE_DOWN;		// 移動方向を下に設定
					enemyMoveCnt = 0;
					return;
				}
			}
		}
	}
}

// 敵の弾処理
//---------------------
void EshotCtl(void)
{
	for (x = 0; x < ENEMY_COUNT_X; x++)
	{
		if (!eShotAlive[x])					// 発射していないとき
		{
			continue;
		}
		else								// 発射しているとき
		{
			eShotPosY[x] += pShotSpeed;		// 弾移動
			if (eShotPosY[x] >= GAME_SCREEN_SIZE_Y - ENEMY_SHOT_SIZE_Y)
			{
				eShotAlive[x] = false;		// 画面外に出たら消滅
			}
		}
	}
}

// 当たり判定処理
//---------------------
void CheckHitCtl(void)
{
	CheckHitPshotEnemy();			// 自機弾と敵の判定
	CheckHitEshotPlayer();			// 敵弾と自機の判定
}

// 自機の弾と敵の当たり判定処理
//---------------------
void CheckHitPshotEnemy(void)
{
	if (pShotAlive)
	{
		for (y = 0; y < ENEMY_COUNT_Y; y++)				// 行：横	列：縦
		{
			for (x = 0; x < ENEMY_COUNT_X; x++)
			{
				if (enemyAlive[y][x])			// 敵が生きていれば処理
				{
					if ((pShotPosY <= enemyPosY[y][x] + ENEMY_SIZE_Y) && 
						(pShotPosY + PLAYER_SHOT_SIZE_Y >= enemyPosY[y][x]) && 
						(pShotPosX <= enemyPosX[y][x] + ENEMY_SIZE_X) && 
						(pShotPosX + PLAYER_SHOT_SIZE_X >= enemyPosX[y][x]))		// 当たり判定
					{
						// 敵を倒した時の処理
						//---------------------
						AddScr(ENEMY_POINT);				// スコア加算
						pShotAlive = false;					// 自機弾消滅
						enemyAlive[y][x] = false;			// 敵撃破
						return;
					}
				}
			}
		}
	}
}

// 自機と敵の当たり判定処理
//---------------------
void CheckHitEshotPlayer(void)
{
	for (int n = 0; n < ENEMY_COUNT_X; n++)
	{
		if (eShotAlive[n])
		{
			if (playerAlive)			// 自機が生きていれば処理
			{
				if ((eShotPosY[n] <= playerPosY + PLAYER_SIZE_Y) && 
					(eShotPosY[n] + ENEMY_SHOT_SIZE_Y >= playerPosY) && 
					(eShotPosX[n] <= playerPosX + PLAYER_SIZE_X) && 
					(eShotPosX[n] + ENEMY_SHOT_SIZE_X >= playerPosX))			// 当たり判定
				{
					// 当たった時の処理
					//---------------------
					eShotAlive[n] = false;		// 敵弾消滅
					DethProcPlayer();			// 自機死亡処理
					return;
				}
			}
		}
	}
}

// 右スコア欄表示処理
//---------------------
void ScrDraw(void)
{
	DrawCount(2, "SCORE", scrData, "HI SCORE");			// 今回のスコアデータ
	DrawCount(6, "HI-SCORE", hiScrData, "HI-SCORE");	// ハイスコアデータ
}

// スコア表示処理
//---------------------
void DrawCount(int rows, const char* str, int Cnt, const char* lenStr)
{
	DrawString(UI_DRAW_X, GAME_SCREEN_Y + GAME_FONT_SIZE * rows, str, 0xffffff);						// スコア表示
	int ScrWidth = GetDrawStringWidth(lenStr, strlen(lenStr));						// SCOREの文字の長さを取得
	int ScrCntWidth = GetDrawFormatStringWidth("%d", Cnt);							// scrDataの数値の長さを取得
	DrawFormatString(UI_DRAW_X + ScrWidth - ScrCntWidth, GAME_SCREEN_Y + GAME_FONT_SIZE * (rows + 1), 0xffffff, "%d", Cnt);					// スコア数字表示			座標に近いものから足していく
}

// スコア加算処理
//---------------------
bool AddScr(int num)
{
	scrData += num;					// スコア加算
	if (scrData > hiScrData)				// 現在のスコアがハイスコアを超えた場合
	{
		hiScrData = scrData;		// ハイスコア更新
	}
	return true;
}

// ゲーム描画処理
//---------------------
void GameDraw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);	// 描画する画面を裏の画面に設定
	ClearDrawScreen();				// 描画する画面の内容を消去

	DrawBox(GAME_SCREEN_X, GAME_SCREEN_Y, GAME_SCREEN_X + GAME_SCREEN_SIZE_X, GAME_SCREEN_Y + GAME_SCREEN_SIZE_Y, GetColor(30, 30, 30), true);			// ゲームスクリーン

	DrawLine(GAME_SCREEN_X, GAME_SCREEN_Y + GET_ENEMY_POS_Y(ENEMY_COUNT_Y + 3), GAME_SCREEN_X + GAME_SCREEN_SIZE_X, GAME_SCREEN_Y + GET_ENEMY_POS_Y(ENEMY_COUNT_Y + 3), 0xffffff);			// ボーダーライン

	for (int n = 0; n < playerLife; n++)
	{
		DrawGraph(UI_DRAW_X + PLAYER_SIZE_X * n, GAME_SCREEN_Y + GAME_SCREEN_SIZE_Y - PLAYER_SIZE_Y, playerImage[DIR_DOWN][1], true);					// 残機表示
	}

	eAnimCnt = abs(((enemyAnimCnt / 10) % 4) - 2);						// 0.1.2.1.0.1.2.1.0........
	if ((eAnimCnt >= 0) && (eAnimCnt < ANIM_MAX))
	{
		for (y = 0; y < ENEMY_COUNT_Y; y++)				// 行：横	列：縦
		{
			for (x = 0; x < ENEMY_COUNT_X; x++)
			{
				// 生存中敵表示
				if (enemyAlive[y][x] == true)
				{
					DrawGraph(GAME_SCREEN_X + enemyPosX[y][x], GAME_SCREEN_Y + enemyPosY[y][x], enemyImage[y % (ENEMY_TYPE_MAX - 1)][enemyDir[y][x]][eAnimCnt], true);
				}
			}
		}
	}

	if (pShotAlive)
	{
		// 生存中自機弾表示
		DrawGraph(GAME_SCREEN_X + pShotPosX, GAME_SCREEN_Y + pShotPosY, pShotImage, true);
	}

	pAnimCnt = abs(((playerAnimCnt / 15) % 4) - 2);							// 0.1.2.1.0.1.2.1.0........
	if ((pAnimCnt >= 0) && (pAnimCnt < ANIM_MAX))
	{
		// 自機表示
		DrawGraph(GAME_SCREEN_X + playerPosX, GAME_SCREEN_Y + playerPosY, playerImage[DIR_UP][pAnimCnt], true);
	}

	for (x = 0; x < ENEMY_COUNT_X; x++)
	{
		if (eShotAlive[x])					// 自機より優先度高め
		{
			// 敵の弾表示
			DrawGraph(GAME_SCREEN_X + eShotPosX[x], GAME_SCREEN_Y + eShotPosY[x], eShotImage, true);
		}
	}

	ScrDraw();			// スコア表示

	_dbgAddDraw();
	ScreenFlip();			// 裏の画面を表の画面に瞬間コピー

	// アニメーションカウント
	enemyAnimCnt++;
	playerAnimCnt++;
}

// ゲームオーバー画面
//---------------------
void GameOverScene(void)
{
	GameOverDraw();

	// カーソル移動
	if (keyBuf[KEY_INPUT_UP] && !keyBufOld[KEY_INPUT_UP])
	{
		gameOverCnt = 2;
	}
	if (keyBuf[KEY_INPUT_DOWN] && !keyBufOld[KEY_INPUT_DOWN])
	{
		gameOverCnt = 4;
	}

	// リトライ
	if (gameOverCnt == 2)
	{
		if (keyBuf[KEY_INPUT_Z] && !keyBufOld[KEY_INPUT_Z])
		{
			GameInit();
			scnID = SCENE_ID_GAME;
		}
	}
	// タイトルに移動
	if (gameOverCnt == 4)
	{
		if (keyBuf[KEY_INPUT_Z] && !keyBufOld[KEY_INPUT_Z])
		{
			scnID = SCENE_ID_TITLE;
		}
	}
}


// ゲームオーバー描画処理
//---------------------
void GameOverDraw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);
	ClearDrawScreen();

	DrawBox(GAME_SCREEN_X, GAME_SCREEN_Y, GAME_SCREEN_X + GAME_SCREEN_SIZE_X, GAME_SCREEN_Y + GAME_SCREEN_SIZE_Y, GetColor(30, 30, 30), true);			// ゲームスクリーン

	DrawGraph(gameOverPosX, gameOverPosY, gameOverImage, true);					// ゲームオーバーロゴ描画

	int ScrWidth = GetDrawStringWidth("CONTINUE?", strlen("CONTINUE?"));
	DrawString(GAME_SCREEN_X + (GAME_SCREEN_SIZE_X - ScrWidth) / 2, GAME_SCREEN_Y + (GAME_SCREEN_SIZE_Y / 2) + GAME_FONT_SIZE, "CONTINUE?",0xff0000);					// "CONTINUE?"の文字列を描画

	ScrWidth = GetDrawStringWidth("YES", strlen("YES"));
	DrawString(GAME_SCREEN_X + (GAME_SCREEN_SIZE_X - ScrWidth) / 2, GAME_SCREEN_Y + (GAME_SCREEN_SIZE_Y * 2 / 3) + GAME_FONT_SIZE * 2, "YES", 0x00ff00);				// "YES"の文字列を描画
	ScrWidth = GetDrawStringWidth("NO", strlen("NO"));
	DrawString(GAME_SCREEN_X + (GAME_SCREEN_SIZE_X - ScrWidth) / 2, GAME_SCREEN_Y + (GAME_SCREEN_SIZE_Y * 2 / 3) + GAME_FONT_SIZE * 4, "NO", 0x00ff00);					// "NO"の文字列を描画

	if ((blinkCnt / 30) % 2)
	{
		ScrWidth = GetDrawStringWidth("⇒", strlen("⇒"));
		DrawString(GAME_SCREEN_X + (GAME_SCREEN_SIZE_X - ScrWidth) / 2 - 30, GAME_SCREEN_Y + (GAME_SCREEN_SIZE_Y * 2 / 3) + GAME_FONT_SIZE * gameOverCnt, "⇒", 0x00ff00);			// "⇒"の文字列を描画

	}
	ScrDraw();			// スコア表示
	ScreenFlip();
	blinkCnt++;
}

// システム系処理
//---------------------
bool SysInit(void)
{
	// システム処理
	//-------------------
	SetWindowText("インベーダー");	// ゲームウィンドウのタイトル
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);			// ゲームウィンドウのサイズと色モードを設定
	ChangeWindowMode(true);				// ゲームウィンドウの表示方法 (true : window     false : フルスクリーン)
	if (DxLib_Init() == -1)				// DXライブラリの初期化処理		Init:イニシャライズ
	{
		TRACE("DxLib の初期化失敗");
		return false;						// DXライブラリの初期化失敗の為システム終了
	}

	srand((unsigned int)time(NULL));

	memset(keyBuf, 0, sizeof(keyBuf));					// keyBufすべてに０をセット		要素数 sizeof(keyBuf) * バイト数 sizeof(keyBuf[0])
	memset(keyBufOld, 0, sizeof(keyBufOld));

	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);

	// グラフィックの登録
	//--------------------
	const char* enemyFileName[ENEMY_TYPE_MAX] = {			// 敵の種類データ
		"image/Enemy 15-1.png",
		"image/Enemy 15-2.png",
		"image/Enemy 15-3.png",
		"image/Enemy 15-4.png",
		"image/Enemy 15-5.png",
		"image/Enemy 15-6.png",
		"image/Enemy 09-1.png"
	};

	titleImage = LoadGraph("image/TitleLogo2.png");				// タイトル画像読み込み
	if (titleImage == -1)
	{
		TRACE("タイトル画像の読み込みに失敗しました")
	}

	gameOverImage = LoadGraph("image/GameOver.png");		// ゲームオーバー画像読み込み
	if (gameOverImage == -1)
	{
		TRACE("ゲームオーバー画像の読み込みに失敗しました")
	}

	// 敵の種類分読み込み
	for (int n = 0; n < ENEMY_TYPE_MAX; n++)
	{
		// 敵のアニメーション分割読み込み
		if (LoadDivGraph(enemyFileName[n], ANIM_MAX * DIR_MAX, ANIM_MAX, DIR_MAX, ENEMY_SIZE_X, ENEMY_SIZE_Y, &enemyImage[n][0][0]) == -1)
		{
			return false;
		}
	}

	pShotImage = LoadGraph("image/pshot.png");				// 自機の弾読み込み
	if (pShotImage == -1)
	{
		TRACE("弾画像の読み込みに失敗しました");
	}
											// 値渡し																// ポインター渡し
	if (LoadDivGraph("image/Male 05-1.png", ANIM_MAX * DIR_MAX, ANIM_MAX, DIR_MAX, PLAYER_SIZE_X, PLAYER_SIZE_Y, &playerImage[0][0]) == -1)		// 自機読み込み
	{
		return false;
	}

	eShotImage = LoadGraph("image/eshot.png");				// 敵の弾表示
	if (eShotImage == -1)
	{
		TRACE("弾画像の読み込みに失敗しました");
	}

	// 変数の初期化
	//--------------------
	scnID = SCENE_ID_TITLE;							// 初期シーン設定

	scrData = 0;						// 初期スコアデータ
	hiScrData = 2000;					// ハイスコアデータ

	blinkCnt = 0;						// 点滅表示カウンタ
	SetFontSize (GAME_FONT_SIZE);		// フォントサイズ設定

	titlePosX = GAME_SCREEN_X + ((GAME_SCREEN_SIZE_X - TITLE_SIZE_X) / 2);		// タイトル画像X座標初期化
	titlePosY = GAME_SCREEN_Y + ((GAME_SCREEN_SIZE_Y - TITLE_SIZE_Y) / 3);		// タイトル画像Y座標初期化

	gameOverPosX = GAME_SCREEN_X + ((GAME_SCREEN_SIZE_X - GAMEOVER_SIZE_X) / 2);		// ゲームオーバー画像X座標初期化
	gameOverPosY = GAME_SCREEN_Y + ((GAME_SCREEN_SIZE_Y - GAMEOVER_SIZE_Y) / 4);		// ゲームオーバー画像Y座標初期化

	return true;
}

// ゲームシステム系処理
//---------------------
bool GameInit(void)
{
	// 変数の初期化
	//--------------------
	scrData = 0;						// 初期スコアデータ

	gameOverCnt = 2;					// ゲームオーバー時のカーソル位置

	playerPosX = (GAME_SCREEN_SIZE_X - PLAYER_SIZE_X) / 2;		// 自機X座標
	playerPosY = GAME_SCREEN_SIZE_Y - PLAYER_SIZE_Y;		// 自機Y座標
	playerAlive = true;						// 自機生存フラグ
	playerSpeed = 4;						// 自機の移動スピード
	playerAnimCnt = 0;					// 自機のアニメーションカウンタ
	playerLife = 2;						// 現在の自機とは別に2体 => 3体

	for (y = 0; y < ENEMY_COUNT_Y; y++)
	{
		for (x = 0; x < ENEMY_COUNT_X; x++)
		{
			enemyPosX[y][x] = (x * ENEMY_SIZE_X) * 3 / 2;		// 敵X座標
			enemyPosY[y][x] = GET_ENEMY_POS_Y(y + 2);		// 敵Y座標
			enemyAlive[y][x] = true;			// 敵生存フラグ
			enemyDir[y][x] = DIR_DOWN;			// 敵の初期移動方向
		}
	}
	enemySpeed = 17;						// 敵の移動スピード
	enemyMoveCnt = 0;						// 敵の移動カウンタ
	enemyAnimCnt = 0;						// 敵のアニメーションカウンタ
	enemyMoveFlagX = MOVE_RIGHT;			// 敵の左右移動管理
	enemyMoveMode = MOVE_MODE_LR;			// 移動方向が　左右or下

	pShotPosX = 0;		// 自機の弾X座標
	pShotPosY = 0;		// 自機の弾Y座標
	pShotAlive = false;	// 自機の弾生存フラグ
	pShotSpeed = 6;		// 自機の弾の移動スピード

	for (x = 0; x < ENEMY_COUNT_X; x++)
	{
		eShotPosX[x] = 0;		// 敵の弾X座標
		eShotPosY[x] = 0;		// 敵の弾Y座標
		eShotAlive[x] = false;	// 敵の弾生存フラグ
	}
	eShotSpeed = 3;				// 敵の弾の移動スピード

	return true;
}
