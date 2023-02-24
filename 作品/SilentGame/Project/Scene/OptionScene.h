#pragma once
#include "BaseScene.h"

template<class T>
class MenuUICtrl;

class OptionBase;

enum class OSELECT
{
	Sound,// 音量
	Key,// キーコンフィグ
	Exit,// 戻る
	Max,
};



class OptionScene :
	public BaseScene
{
public:
	OptionScene(uniqueScene gameScene);
	OptionScene(uniqueScene gameScene,uniqueScene beforScene,int mapNum, Scene scene);
	~OptionScene();
private:
	bool Init(void) override;

	uniqueScene Update(float delta, uniqueScene ownScene) override;
	void DrawOwnScreen(float delta) override;  // 自分自身を描画
 
	Scene GetSceneID(void) override { return Scene::Option; };
	bool GetFlag(void) override { return SceneFlag_; };


	/// <summary> 設定状態の更新 </summary>
	/// <param name="delta"> デルタタイム </param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateSetting(float delta, uniqueScene ownScene);

	/// <summary> 通常状態の時 </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateSelect(float delta, uniqueScene ownScene);

	/// <summary> optionへ移行した時の演出用 </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateOpen(float delta, uniqueScene ownScene);

	/// <summary> optionから他のシーンへ移行するときの処理 </summary>
	/// <param name="delta"></param>
	/// <param name="ownScene"></param>
	/// <returns></returns>
	uniqueScene UpdateClose(float delta, uniqueScene ownScene);

	/// <summary> 解放処理 </summary>
	void Relese();


	/// <summary> 設定状態の時の描画 </summary>
	/// <param name=""></param>
	void DrawSetting(void);

	/// <summary> 通常状態の描画 </summary>
	/// <param name=""></param>
	void DrawSelect(void);

	/// <summary> optionへ移行したときの描画 </summary>
	/// <param name=""></param>
	void DrawOpen(void);

	/// <summary> optionから他のシーンへ移行するときの描画 </summary>
	/// <param name=""></param>
	void DrawClose(void);

	/// <summary> 操作の説明の表示 </summary>
	/// <param name=""></param>
	void DrawInput(void);

	/// <summary> メニューのウィンドウの表示 </summary>
	/// <param name=""></param>
	void DrawMenu(void);

	// uniqueScene beforScene_; // 戻った時のシーン
	uniqueScene beforeScene_;// 描画用のシーン

	// 更新処理のメンバ関数ポインタ
	uniqueScene(OptionScene::* update_)(float, uniqueScene);



	// 描画処理のメンバ関数ポインタ
	void (OptionScene::* draw_)(void);

	// 選択した設定項目のクラス
	std::unique_ptr< OptionBase> optionWnd_;

	// メニューUIを管理するクラス
	std::unique_ptr< MenuUICtrl<OSELECT>> menuUICtrl_;

	Scene scene_;

	// 経過時間
	float time_;

	// ゲームシーンから来たか?
	bool isBeforGameScene_;
 
	// optionのウィンドウのスクリーンハンドル
	int wndScreenH_;
};

