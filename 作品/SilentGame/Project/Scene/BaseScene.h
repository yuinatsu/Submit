#pragma once
#include<memory>
#include"../common/Math.h"
#include "../Input/Controller.h"

class BaseScene;
using uniqueScene = std::unique_ptr<BaseScene>;

/// <summary> シーンの種類 </summary>
enum class Scene
{
	Transtion,				// シーン遷移
	Title,					// タイトル
	Tutorial,				// チュートリアル
	Select,					// セレクト
	Game,					// ゲーム
	Clear,					// クリア
	Pause,					// ポーズ
	Option,					// オプション
	Result,					// リザルト
	Load,					// ロード
	Max
};

class BaseScene
{
public:
	/// <summary> コンストラクタ </summary>
	BaseScene();
	virtual ~BaseScene();

	/// <summary> 初期化 </summary>
	/// <returns> 正常動作:true </returns>
	virtual bool Init(void) = 0;

	/// <summary> 更新 </summary>
	/// <param name="delta"> デルタタイム </param>
	/// <param name="ownScene"> 現在のシーン </param>
	/// <returns> 現在のシーン </returns>
	virtual uniqueScene Update(float delta, uniqueScene ownScene) = 0;

	/// <summary> 描画 </summary>
	/// <param name="delta"> デルタタイム </param>
	virtual void Draw(float delta);

	virtual void Draw(float delta, Math::Vector2 pos, double extRate);

	/// <summary> 描画 </summary>
	/// <param name="extRate"> 拡大率 </param>
	/// <param name="angle"> 回転角度 </param>
	virtual void Draw(Math::Vector2I pos, double extRate, double angle);

	/// <summary> 現在のシーンの描画 </summary>
	/// <param name="delta"> デルタタイム </param>
	virtual void DrawOwnScreen(float delta) = 0;

	/// <summary> 現在のシーンの取得 </summary>
	/// <returns> 現在のシーン </returns>
	virtual Scene GetSceneID(void) = 0;

	/// <summary> シーン遷移フラグ取得 </summary>
	/// <returns> シーン遷移:true </returns>
	virtual bool GetFlag(void) = 0;

	virtual void SoundPlay(void);

	/// <summary> 角度取得 </summary>
	/// <returns> 角度 </returns>
	virtual double GetAngle(void) { return angle_; }

	/// <summary> ロード終了時の処理 </summary>
	/// <param name=""></param>
	virtual void Loaded(void);
protected:
	/// <summary> シーン遷移フラグ </summary>
	bool SceneFlag_;

	/// <summary> 画面サイズ </summary>
	Math::Vector2I screenSize_;

	/// <summary> シーンデータの格納 </summary>
	int screenID_;

	/// <summary> コントローラ情報 </summary>
	std::unique_ptr<Controller> controller_;

	/// <summary> ステージ番号 </summary>
	int mapNum_;

	/// <summary> 拡大率 </summary>
	double extRate_;

	/// <summary> 角度 </summary>
	double angle_;
	
	/// <summary> サウンドが流れて何秒立ったか /// </summary>
	float loopTime_;
};

