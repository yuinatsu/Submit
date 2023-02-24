#pragma once

// 定数定義
//-----------------
#define SCREEN_SIZE_X 800			// 画面の横サイズ
#define SCREEN_SIZE_Y 600			// 画面の縦サイズ

#define GAME_SCREEN_X 40			// ゲーム画面の描画オフセット
#define GAME_SCREEN_Y 75			// ゲーム画面の描画オフセット
#define GAME_SCREEN_SIZE_X 640		// ゲーム画面の横サイズ
#define GAME_SCREEN_SIZE_Y 480		// ゲーム画面の縦サイズ

#define GAME_FONT_SIZE 18			// フォントのサイズ

#define TITLE_SIZE_X 423			// タイトルの横サイズ
#define TITLE_SIZE_Y 198			// タイトルの縦サイズ

#define GAMEOVER_SIZE_X 452			// ゲームオーバーロゴの横サイズ
#define GAMEOVER_SIZE_Y 123			// ゲームオーバーロゴの縦サイズ

#define ENEMY_POINT 100				// 敵の点数

#define PLAYER_SIZE_X 32			// 自機の横サイズ
#define PLAYER_SIZE_Y 32			// 自機の縦サイズ

#define PLAYER_SHOT_SIZE_X 6		// 自機の弾の横サイズ
#define PLAYER_SHOT_SIZE_Y 30		// 自機の弾の縦サイズ

#define ANIM_MAX 3					// アニメーションのコマ数

#define ENEMY_SIZE_X 32				// 敵の横サイズ
#define ENEMY_SIZE_Y 32				// 敵の縦サイズ

#define ENEMY_SHOT_SIZE_X 16		// 敵の弾の横サイズ
#define ENEMY_SHOT_SIZE_Y 25		// 敵の弾の縦サイズ

#define ENEMY_COUNT_X 8				// 敵の横並びの数
#define ENEMY_COUNT_Y 7				// 敵の縦並びの数
#define ENEMY_MOVE_INT 5			// 敵の移動間隔

#define GET_ENEMY_POS_Y(y) (((y) * ENEMY_SIZE_Y) * 9 / 7)					// 敵のY座標

#define UI_DRAW_X (GAME_SCREEN_X + GAME_SCREEN_SIZE_X + 20)					// 画面右の表示欄　X座標

// 方向
enum DIR_ID {
	DIR_DOWN,			// 下方向
	DIR_LEFT,			// 左方向
	DIR_RIGHT,			// 右方向
	DIR_UP,				// 上方向
	DIR_MAX
};

// 敵種類
enum ENEMY_TYPE {
	ENEMY_TYPE_1,
	ENEMY_TYPE_2,
	ENEMY_TYPE_3,
	ENEMY_TYPE_4,
	ENEMY_TYPE_5,
	ENEMY_TYPE_6,
	ENEMY_TYPE_UFO,
	ENEMY_TYPE_MAX
};

// 敵の左右移動方向
enum MOVE_LR {
	MOVE_LEFT,			// 左方向
	MOVE_RIGHT,			// 右方向
	MOVE_MAX
};

// 移動モードが左右 or 下
enum MOVE_MODE
{
	MOVE_MODE_LR,		// 左右移動モード
	MOVE_MODE_DOWN,		// 下移動モード
	MOVE_MODE_MAX
};

// シーン画面操作
enum SCENE_ID
{
	SCENE_ID_TITLE,
	SCENE_ID_GAME,
	SCENE_ID_GAMEOVER,
	SCENE_ID_RULE,
	SCENE_ID_MAX
};

// プロトタイプ宣言
//-----------------
void TitleScene(void);					// タイトル画面
void TitleDraw(void);					// タイトル描画処理

void RuleScene(void);					// ルール説明用画面
void RuleDraw(void);					// ルール説明表示

void GameScene(void);					// ゲーム画面
void PlayerCtl(void);					// 自機処理
void PshotCtl(void);					// 自機の弾処理
void EnemyCtl(void);					// 敵処理
void CheckEnemyLine(void);				// 敵の防衛ライン到達確認
bool DethProcPlayer(void);				// 自機の死亡時処理
void CheckEnemyMoveLR(void);			// 敵方向転換確認		void...返り値なし
void EshotCtl(void);					// 敵の弾処理
void CheckHitCtl(void);					// 当たり判定処理
void CheckHitPshotEnemy(void);			// 自機の弾と敵の当たり判定処理
void CheckHitEshotPlayer(void);			// 自機と敵の当たり判定処理
void ScrDraw(void);						// スコアUI表示
void DrawCount(int rows, const char* str, int Cnt, const char* lenStr);			// スコア表示処理
bool AddScr(int num);					// スコア加算処理
void GameDraw(void);					// ゲーム描画処理

void GameOverScene(void);				// ゲームオーバー画面
void GameOverDraw(void);				// ゲームオーバー描画処理

bool SysInit(void);						// システム系初期化		返り値はtrueかfalse
bool GameInit(void);					// ゲームシステム系初期化