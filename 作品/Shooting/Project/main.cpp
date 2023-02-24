//-------------------------------
// ゲーム制作	：Project1
// 氏名			：2016010 梶原尚人
//-------------------------------

#include "DxLib.h"			// DXライブラリの使用
#include "time.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"

// 定数定義
//------------------
#define SCREEN_SIZE_X 640		// 画面の横サイズ
#define SCREEN_SIZE_Y 480		// 画面の縦サイズ
#define BG_SIZE_Y	960			// 背景の縦サイズ
#define PLAYER_SIZE_X 32		// 自機の横サイズ
#define PLAYER_SIZE_Y 32		// 自機の縦サイズ
#define SHOT_SIZE_X 32			// 自機の弾の横サイズ
#define SHOT_SIZE_Y 32			// 自機の弾の縦サイズ
#define ENEMY_SIZE_X 48			// 敵の横サイズ
#define ENEMY_SIZE_Y 48			// 敵の縦サイズ

// 変数定義
//------------------
// 背景関係
//------------------
int bgImage;					// グラフィックハンドル
int bgPosY1;					// 背景１のY座標
int bgPosY2;					// 背景２のY座標
int bgSpeed;					// 移動量

// プレイヤー関連
//------------------
int playerImage;				// グラフィックハンドル
int playerPosX;					// X座標
int playerPosY;					// Y座標
int playerSpeed;				// 移動量
int count;						// 無敵時間カウント
bool playerAlive;				// 生存フラグ

// 弾関連
//------------------
int shotImage;					// グラフィックハンドル
int shotPosX;					// X座標
int shotPosY;					// Y座標
int shotSpeed;					// 移動量
bool shotAlive;					// 発射フラグ
double shotRad;					// 角度

// 敵関連
//------------------
int enemyImage;					// グラフィックハンドル
int enemyPosX;					// X座標
int enemyPosY;					// Y座標
int enemySpeed;					// 移動量
bool enemyAlive;				// 生存フラグ

// WinMain関数
//------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// システム処理
	//-------------------
	SetWindowText("シューティング");						// ゲームウィンドウのタイトル
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);			// ゲームウィンドウのサイズと色モードを設定
	ChangeWindowMode(true);									// ゲームウィンドウの表示方法 (true : window     false : フルスクリーン)
	if (DxLib_Init() == -1)									// DXライブラリの初期化処理		Init:イニシャライズ
	{
		TRACE("DxLib の初期化失敗");
		return -1;											// DXライブラリの初期化失敗の為システム終了
	}

	srand((unsigned int)time(NULL));

	_dbgSetup(SCREEN_SIZE_X, SCREEN_SIZE_Y, 255);

	// グラフィックの登録		ゲーム処理より前に、システム処理より後にしないといけない(DxLib_Initの定義が必要なため)
	//--------------------
	bgImage = LoadGraph("image/haikei.png");
	if (bgImage == -1)
	{
		TRACE("背景画像の読み込みに失敗しました");
	}
	playerImage = LoadGraph("image/player.png");
	if (playerImage == -1)
	{
		TRACE("自機画像の読み込みに失敗しました");
	}

	shotImage = LoadGraph("image/shot.png");
	if (shotImage == -1)
	{
		TRACE("弾画像の読み込みに失敗しました");
	}

	enemyImage = LoadGraph("image/enemy2.png");
	if (enemyImage == -1)
	{
		TRACE("敵画像の読み込みに失敗しました");
	}

	// 変数の初期化
	//--------------------
	bgPosY1 = 0;					// 背景１の初期位置
	bgPosY2 = -BG_SIZE_Y;			// 背景２の初期位置
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

	// ゲームループ
	//--------------------
	while ((ProcessMessage() == 0) && (CheckHitKey(KEY_INPUT_ESCAPE) == 0))
	{
		_dbgStartDraw();
		// ゲームのメイン処理
		//---------------------

		// 背景移動処理
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

		// 自機移動処理
		//---------------------

// 当たり判定①・ここで参照する座標はすべてが１フレーム前

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
					playerPosX += playerSpeed;					// 右移動
				}
				else
				{
					playerPosX = SCREEN_SIZE_X - PLAYER_SIZE_X;		// 右移動制限
				}
			}
			if (CheckHitKey(KEY_INPUT_LEFT) == 1)
			{
				if (playerPosX - playerSpeed >= 0)
				{
					playerPosX -= playerSpeed;					// 左移動
				}
				else
				{
					playerPosX = 0;									// 左移動制限
				}
			}
			if (CheckHitKey(KEY_INPUT_UP) == 1)
			{
				if (playerPosY - playerSpeed >= 0)
				{
					playerPosY -= playerSpeed;					// 上移動
				}
				else
				{
					playerPosY = 0;									// 上移動制限
				}
			}
			if (CheckHitKey(KEY_INPUT_DOWN) == 1)
			{
				if (playerPosY + playerSpeed <= SCREEN_SIZE_Y - PLAYER_SIZE_Y)
				{
					playerPosY += playerSpeed;					// 下移動
				}
				else
				{
					playerPosY = SCREEN_SIZE_Y - PLAYER_SIZE_Y;		// 下移動制限
				}
			}
		}

// 当たり判定②・ここで参照する座標は自機は現フレーム、敵と弾は１フレーム前

		// 敵移動処理
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
			// 乱数で欲しい範囲 : 0 ～ (SCREEN_SIZE_X - ENEMY_SIZE_X)
			enemyPosX = rand() % (SCREEN_SIZE_X - ENEMY_SIZE_X);				// 画面内で乱数
			enemyPosY = -ENEMY_SIZE_Y;
			enemyAlive = true;
		}

		// 弾処理
		//---------------------
		if (shotAlive == false)					// 発射していないとき
		{
			if (CheckHitKey(KEY_INPUT_Z) == 1 && playerAlive == true)
			{
				shotAlive = true;
				shotPosX = playerPosX;
				shotPosY = playerPosY;
			}
		}
		else
		{										// 発射しているとき
			shotPosY -= shotSpeed;
			if (shotPosY <= -SHOT_SIZE_Y)		// 画面外に行ったら
			{
				shotAlive = false;
			}
		}

// 当たり判定③・ここで参照する座標は自機、敵、弾共に現フレーム

		// 当たり判定
		//---------------------
		if ((shotAlive == true) && (enemyAlive == true))
		{
			if ((shotPosY <= enemyPosY + ENEMY_SIZE_Y) && (shotPosY + SHOT_SIZE_Y >= enemyPosY) && (shotPosX <= enemyPosX + ENEMY_SIZE_X) && (shotPosX + SHOT_SIZE_X >= enemyPosX))
			{
				shotAlive = false;
				enemyAlive = false;
				TRACE("当たり\n");
			}
		}

		if ((playerAlive == true) && (enemyAlive == true) && count == 0)
		{
			if ((playerPosY <= enemyPosY + ENEMY_SIZE_Y) && (playerPosY + PLAYER_SIZE_Y >= enemyPosY) && (playerPosX <= enemyPosX + ENEMY_SIZE_X) && (playerPosX + PLAYER_SIZE_X >= enemyPosX))
			{
				playerAlive = false;
				enemyAlive = false;
				TRACE("無敵開始\n");
				count = 450;					// 約三秒間
			}
		}
		if (count > 0)
		{
			count--;
			if (count <= 0)
			{
				TRACE("無敵終了\n");
				count = 0;
				playerAlive = true;
			}
		}

		// 描画処理
		//---------------------
		SetDrawScreen(DX_SCREEN_BACK);	// 描画する画面を裏の画面に設定
		ClsDrawScreen();				// 描画する画面の内容を消去

		DrawGraph(0, bgPosY1, bgImage, false);					// クリア前かつ裏画面コピー後に
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
		ScreenFlip();					// 裏の画面を表の画面に瞬間コピー
	}

	// システム終了処理
	//--------------------
	DxLib::DxLib_End();			// DXライブラリの終了処理
	return 0;			// ゲームの終了
}