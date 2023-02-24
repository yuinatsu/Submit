#pragma once
#include "BaseScene.h"
#include "../common/Vector2.h"
#include "Obj/Obj.h"
#include "Obj/ChipID.h"
#include "Obj/ChipGP.h"
#include <vector>
#include <map>
#include <list>
#include <functional>			// 意味）ちょっとした命令、処理


#define lpMapCtl MapCtl::GetInstance()

using MapChipGP = std::map<ChipID, ChipGP>;

#define LDR_ID_NAME "LDR_MAP_DATA"
#define LDR_ID_VER 0x01					// 初期のバージョン番号は０

#define DigCnt 10													// １コマ当たりのフレーム数
#define DigFrameLim 5												// コマ数
#define DigWait (60 * 4)											// ４秒間の待機時間
#define DigFullFrame ((DigCnt * DigFrameLim) * 2 + DigWait)			// 総フレーム数


struct DataHeader
{
// デフォルトはpublic
	char fileID[13];			// ファイルのID情報						セーブデータの文字列
	unsigned char verID;		// ファイルのバージョン番号				中身をバージョンアップを行ったらIDを増やして古いファイルは使えないようにする
	unsigned char res1[2];		// アライメント分予約領域とする			メモリを4バイトずつ並べるために2バイト足す		reserve ... 予約
	int sizeX;					// マップのXサイズ
	int sizeY;					// マップのYサイズ
	unsigned char res2[3];		// アライメント分予約領域とする			メモリを4バイトずつ並べるために3バイト足す
	unsigned char sum;			// SUM値
};


class MapCtl
{
public:
	static MapCtl& GetInstance(void)
	{
		return (*s_Instance);
	}
	static void Create(SceneID sceneID);
	static void Destroy(void);
	void DrawEditOffset();				// マップ外オフセットの描画

	bool SetMapData(Vector2 pos, ChipID id);						// (座標, チップのID)
	bool SetMapData(Vector2 pos, ChipID id, ChipGP gp);				// (座標, チップのID, )
	ChipID GetMapDataEdit(ChipGP gpID, unsigned int data);				// (, baseMapID_)											指定データからChipIDを取り出す
	ChipID GetMapDataGame(ChipGP gpID, unsigned int data);
	ChipID GetMapData(ChipGP gpID, int x, int y, bool dig);			// (, カーソルのX座標のマス目, カーソルのY座標のマス目, 掘るかどうかのチェック)		指定したマス目からデータを取得し、ChipIDを取り出す
	ChipID GetMapData(ChipGP gpID, Vector2 pos, bool dig);					// (, カーソルの座標)										指定した座標からデータを取得し、ChipIDを取り出す

	bool SetDig(const Vector2& pos, DIR_LR dir);					// (自機座標, 自機の向き)
	int GetDigData(int x, int y);									// 
	void DigUpdate(void);											// Dig情報の更新

	bool EraceGold(Vector2 pos, bool playerFlag);					// Map中からGoldを消す			(金塊の座標, 自機が持っているかのフラグ)

	void InputButtonL(bool input);
	void InputButtonR(bool input);

	void SetCamera(std::weak_ptr<Camera> camera);
	int GetGoldCnt();							// 金塊の値を返す変数
	bool MapSave(void);							// データの保存		拡張する場合は引数にnumを入れて保存場所指定する
	bool MapLoad(std::string fileName);							// データの読み込み				editSceneおよびgameSceneの内部で呼び出す
	bool MapLoad(std::string fileName, BaseScene& scene);				// gameScene
	bool MapDel(void);


	std::function<void(void)> DrawFunc;					// 関数丸ごと持ってきて関数名だけ消せばいい				返り値の型は合わせる
	std::function<ChipID(ChipGP, unsigned int)> GetMapDataFunc;

	Vector2 GetDrawOffset();
private:
	bool CheckMapChar(ChipID id);			//プレイヤー設置数制限
	void EditDraw(void);					// EditScene用のDraw関数
	void GameDraw(void);					// GameScene用のDraw関数
	void DrawMap(void);						// マップに表示するマップチップ			GameDraw及びEditDraw関数で内部的に呼び出す
	void DrawEditLine(void);				// マップに表示するライン			EditDraw関数で内部的に呼び出す
	MapCtl(SceneID sceneID);				// MapCtlのインストラクタ
	~MapCtl();								// MapCtlのデストラクタ
	bool Init(void);						// MapCtlのInit
	bool InitMap(void);						// マップ系のInit
	bool InitDig(void);						// 掘る系のInit
	void InitDrawChipEdit();				// EditSceneのDrawChip_の初期化
	void InitDrawChipGame();				// GameSceneのDrawChip_の初期化
	static MapCtl* s_Instance;				// MapCtlのインスタンス
	SceneID sceneID_;						// sceneIDを保存するための器

	Vector2 viewArea_;						// １画面に表示されるマップの広さ
	Vector2 worldArea_;						// マップの広さ
	Vector2 chipSize_;						// チップのサイズ
	Vector2 drawOffset_;					// 描画オフセット

	std::map<ChipID, std::function<void(const int x, const int y, const Vector2 & chipSize, const Vector2 & drawOffset, const int imageID)>> DrawChip_;


	// ２次元配列
	//-------------
	std::vector<unsigned int> baseMapID_;
	std::vector<unsigned int*> mapID_;								// mapID_の中のポインタを使用してbaseMapID_にアクセスする		ポインタの可変性

	std::vector<unsigned int> baseMapDig_;							// 
	std::vector<unsigned int*> mapDig_;								// Dig ... 掘る
	std::list<Vector2> digPosList_;									// 掘った位置の記憶リスト

	// std::vector<std::vector<unsigned int>> mapID_;				// vector内にvectorを入れることによって２次元配列が実現可能（普通の２次元配列と違いvector１つひとつのよって要素数を変えられる）


	// 変数
	//-------
	MapChipGP chipGP_;

	int CrL_;					// 左矢印の色
	int CrR_;					// 右矢印の色
	int goldCnt_;				// MAP内の金塊の数
	int animCnt_;				// 背景アニメーション用カウンタ

	std::weak_ptr<Camera> camera_;		// Cameraのweak情報

	// 友達名簿
	//-----------
	friend class Camera;				// CameraがMapCtlのprivateにさわれるようにする		「お前のものは俺のもの」
	friend class GameScene;
};
