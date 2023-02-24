#pragma once
#include <list>
#include <memory>
#include "BaseScene.h"
#include "../Obj/Obj.h"
#include "../../Tiled/TmxObj.h"
class TmxObj;
class Player;

class GameScene:
	public BaseScene
{
public:
	static constexpr int StartCallMax = 2;									// カウントダウンロゴ総数
	static constexpr double CallCountMax = 1.0;								// カウントダウンの間隔
	GameScene();
	~GameScene();
private:
	std::shared_ptr<TmxObj> tmxObj_;										// ステージデータ(tmx)
	std::list<std::unique_ptr<Obj>> objList_;								// Objのリスト

	bool Init(void) override;												// 初期化
	uniqueBaseScene Update(double delta, uniqueBaseScene own) override;		// 更新 Update(deltaTime, 各シーン)
	void DrawOwnScreen(double delta);										// 各シーンの描画
	Scene GetSceneID(void) override { return Scene::Game; };				// 各シーンの取得
	void CallDraw();													// ゲーム開始のカウントダウン描画

	bool bflag_;															// ボタンフラグ
	Stage stage_;															// ステージ情報格納用
	int stageNum_;
	bool clearFlag_;														// クリア判定
	int pauseNum_;

	// カウントダウン
	double count_;
	bool callFlag_;															// コールの有無のフラグ

	int clearCnt_;
	bool playerFlag_;

	int gameBGM1_;
	int gameBGM2_;
	int gameBGM3_;
	int selectSE_;
};

