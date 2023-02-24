#pragma once

enum class SceneID
{
	Title,		// タイトル
	Select,		// 選択
	Game,		// ゲーム中
	Result,		// 結果
	Pause,		// ポーズ
	Option,		// オプション
	Transition,	// 遷移
	Loading,	// ロード中
};

enum class ResultAttribute
{
	Clear,
	GameOver,
	Max,
};
