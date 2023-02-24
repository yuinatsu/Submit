#pragma once

// シーンID
enum class SceneID
{
	NON,						// 何もしない
	TRANSITION,					// 画面移行中
	EDIT,						// 編集画面
	GAME,						// ゲーム実行画面
	CLEAR,						// クリア画面
	MAX
};