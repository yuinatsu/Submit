#pragma once
#include <memory>
#include <list>
#include <vector>
#include <fstream>
#include "../Scene/SceneID.h"
#include "../Common/Vector2.h"

class BaseScene;
class ObjectManager;
class UiBase;
class Controller;

class UiManager
{
public:
	UiManager(SceneID scene);
	UiManager(SceneID scene, ResultAttribute atr);
	~UiManager();
	void InitUI(SceneID scene);
	void Begin();
	void Update(float delta, BaseScene& scene, ObjectManager& obj, Controller& controller);
	void Draw(void);

	const std::list<std::unique_ptr<UiBase>>& GetUiList(void) const { return uiList_; }
private:
	// タイトルシーンの作成
	void CreateTitleSceneUI(Vector2 pos, float interval);

	// セレクトシーンのUIの作成
	void CreateSelectSceneUI(Vector2 pos, float interval);

	// ゲームシーンのUIの作成
	void CreateGameSceneUI(Vector2 gaugePos, float interval, Vector2 comboPos);

	// リザルトシーンUIの作成
	void CreateResultSceneUi(Vector2 pos, float interval);

	// ロードシーンUIを作成
	void CreateLoadSceneUI(Vector2 pos);

	// ポーズシーンUIを作成
	void CreatePauseSceneUI(Vector2 pos, float interval);

	/// <summary>
	/// オプションシーンのUI
	/// </summary>
	/// <param name=""></param>
	void CreateOptionSceneUI(void);

	// UI用のリスト
	std::list<std::unique_ptr<UiBase>> uiList_;
	// 呼び出したシーン
	SceneID scene_;
	// リザルト結果
	ResultAttribute result_;
};

