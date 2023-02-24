#pragma once
#include <memory>
#include <functional>
#include <list>
#include "TitleScene.h"
#include "BaseScene.h"

enum class PSELECT
{
	Resume,// 再開
	Retry,// リトライ
	SelectScene,// セレクトシーンに遷移
	Item,
	Setting,// 設定
	Suspension,// ゲーム終了
	Max,
};
template<class T>
class MenuUICtrl;

class ItemDescriptionWnd;

class PauseScene :
	public BaseScene
{
public:
	PauseScene(uniqueScene beforScene, int mapNum, bool isSmallMap,Scene scene);
	~PauseScene();
private:
	bool Init(void) override;
	uniqueScene Update(float delta, uniqueScene ownScene) override;
	
	/// <summary> 選択画面のアップデート </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateSetting(float delta, uniqueScene ownScene);

	/// <summary> ゲーム終了を選択時 </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateEnd(float delta, uniqueScene ownScene);

	/// <summary> アイテム説明を選択時 </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateWnd(float delta, uniqueScene ownScene);

	/// <summary> ポーズ画面を開いた時のアップデート </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateOpen(float delta, uniqueScene ownScene);

	/// <summary> ポーズ画面を閉じる時のアップデート </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateClose(float delta, uniqueScene ownScene);


	/// <summary>  </summary>
	/// <param name="delta"></param>
	void UpdateConfirmation(float delta);
	
	
	void DrawOwnScreen(float delta) override;  // 自分自身を描画
	Scene GetSceneID(void) override { return Scene::Pause; };    // 自分はポーズシーン
	bool GetFlag(void) override { return SceneFlag_; };
	
	/// <summary> 選択画面のDraw </summary>
	/// <param name="delta"></param>
	void DrawSetting(float delta);
	
	/// <summary> 終了選択時の描画 </summary>
	/// <param name="delta"></param>
	void DrawEnd(float delta);

	/// <summary> アイテム説明選択時の描画 </summary>
	/// <param name="delta"></param>
	void DrawWnd(float delta);

	/// <summary> ポーズ画面を開いた時の描画 </summary>
	/// <param name="delta"></param>
	void DrawOpen(float delta);

	/// <summary> ポーズ画面を閉じる時の描画 </summary>
	/// <param name="delta"></param>
	void DrawClose(float delta);
	
	/// <summary>  </summary>
	/// <param name=""></param>
	void DrawKeyUD(void);

	/// <summary>  </summary>
	/// <param name=""></param>
	void DrawKeyLR(void);

	/// <summary>  </summary>
	/// <param name=""></param>
	void DrawItemKeyAll(void);

	/// <summary>  </summary>
	/// <param name=""></param>
	void DrawItemKeyEnd(void);

	
	void Relese(void);
	

	//uniqueScene (PauseScene::* updata_)(float, uniqueScene);
	//void (PauseScene::* draw_)(float);

	// アップデート用メンバ関数ポインタ
	uniqueScene (PauseScene::* updateFunc_)(float, uniqueScene);

	// 描画処理用メンバ関数ポインタ
	void (PauseScene::* drawFunc_)(float);


	uniqueScene beforScene_;

	// ?
	int scene_;

	float time_;

	Scene oldScene_;

	// 次がオプションシーンかどうか
	bool isNextOption_;

	// メニューのUIをコントロールするクラス
	std::unique_ptr<MenuUICtrl<PSELECT>> menuUICtrl_;

	// アイテム説明用のウィンドウっぽいもの
	std::unique_ptr< ItemDescriptionWnd> itemWnd_;

	std::unique_ptr<MenuUICtrl<TST>> tstUICtrl_;

	// ポーズ画面のスクリーンハンドル
	int pauseWndScreenH_;

	bool isSmallMap_;
};

