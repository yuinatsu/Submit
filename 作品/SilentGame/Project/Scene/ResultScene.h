#pragma once
#include "BaseScene.h"
#include "../common/Math.h"

enum class RSELECT
{
	Retry,// もう一度挑戦
	Select,// セレクトシーンに遷移
	Title,// タイトルシーン
	Max,
};

class ResultData;
class ResultDrawCtrl;

template<class T>
class MenuUICtrl;

class ResultScene :
	public BaseScene
{
public:
	ResultScene(ResultData& result,int mapNum, bool isSmallMap , float delta);
	~ResultScene();
private:
	bool Init(void) override;
	// 描画などの初期化
	void InitScreen(void);

	void Relese(void);

	uniqueScene Update(float delta, uniqueScene ownScene) final;

	// 選択画面のUpdate
	uniqueScene UpdateSelect(float delta, uniqueScene ownScene);

	void DrawOwnScreen(float delta) final;  // 自分自身を描画

	void DrawSelect(float delta);
	
	Scene GetSceneID(void) override { return Scene::Result; };    // 自分はリザルトシーン
	
	bool GetFlag(void) override { return SceneFlag_; };
	
	// 結果の表示をコントロールするクラス
	std::unique_ptr< ResultDrawCtrl> resultDrawCtrl_;

	// メニューUIをコントロールするクラス
	std::unique_ptr<MenuUICtrl<RSELECT>> menuUICtrl_;

	bool isSmallMap_;

	// 仮置き
	float count_;
};

