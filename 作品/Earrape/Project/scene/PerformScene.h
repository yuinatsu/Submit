#pragma once
#include "DataBaseScene.h"
class PerformScene :
	public DataBaseScene
{
public:
	PerformScene();
	~PerformScene();
	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void Draw(void) override;	// グラフィックの描画
	bool Init(void) override;	// 初期化
	bool Release(void) override;	// リソースの解放
private:
	int itemNum_;			// 項目のナンバー
	int ItemImage_[static_cast<int> (Dataitem::MAX)];
	int perImage_[static_cast<int> (PERFORM::MAX)];
	int nonImage_;

	Vector2 iconStart;
	Vector2 iconSpace;
	Vector2 perSpace;

	Vector2 pos_;
	int iconoffset;
	bool effectflag;
};

