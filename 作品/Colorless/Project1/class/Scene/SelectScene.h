#pragma once
#include "BaseScene.h"
#include "../Obj/Obj.h"
#include "../../Tiled/TmxObj.h"
class TmxObj;
class Player;

class SelectScene :
    public BaseScene
{
public:
	static constexpr int CursorRangeUp = 150;								// カーソル移動制限上部
	static constexpr int CursorRangeDown = 550;								// カーソル移動上限下部
	SelectScene();
	~SelectScene();
private:
	bool Init(void) override;												// 初期化
	uniqueBaseScene Update(double delta, uniqueBaseScene own) override;		// 更新 Update(deltaTime, 各シーン)
	void DrawOwnScreen(double delta);										// 各シーンの描画
	Scene GetSceneID(void) override { return Scene::Select; };				// 各シーンの取得

	std::shared_ptr<TmxObj> tmxObj_;										// ステージデータ(tmx)
	std::list<std::unique_ptr<Obj>> obj_;									// Objのリスト
	bool selectFlag_;														// セレクト判定
	bool bflag_;															// ボタンフラグ
	int stageNum_;															// ステージナンバー

	int pauseNum_;
	int startCnt_;
	double count_;																	// 1フレームあたりの値
	bool playerFlag_;
	bool logoFlag_;

	int selectBGM_;
	int selectSE_;
};

