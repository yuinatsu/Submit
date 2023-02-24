#pragma once
#include <memory>
#include "BaseScene.h"
#include "../common/Vector2.h"

#define lpSceneMng SceneMng::GetInstance()		// インスタンス取得

class SceneMng
{
public:
	// インスタンス取得
	static SceneMng& GetInstance()
	{
		static SceneMng s_Instance;			// インスタンス生成
		return s_Instance;
	}
	void Run(void);							// ゲームループ
	void Draw(void);						// 描画
	const Vector2 scnArea_;					// 画面解像度
	const Vector2 worldArea_;				// ゲームの世界のチップ数
	const Vector2 viewArea_;				// 表示チップ数
	const Vector2 chipSize_;				// １チップ当たりのドット数
	const Vector2 drawOffset_;				// 画面左上からのゲームエリア
private:
	uniqueBaseScene scene_;					// シーン情報		// BaseScene* scene_ に unique_ptrを含めた
	void Init(void);						// 初期化
	bool SysInit(void);						// システム初期化
	SceneMng();
	~SceneMng();
};

