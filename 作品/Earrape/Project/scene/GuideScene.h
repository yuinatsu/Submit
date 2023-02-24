#pragma once
#include "BaseScene.h"
#include "../SCN_ID.h"
#include "../common/Vector2.h"

enum class GuidePage
{
	PAGE_1,
	PAGE_2,
	PAGE_3,
	PAGE_4,
	PAGE_5,
	MAX
};

class GuideScene :
    public BaseScene
{
public:
	GuideScene();
	~GuideScene();
	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void Draw(void) override;	// グラフィックの描画
	bool Init(void) override;	// 初期化
	bool Release(void) override;	// リソースの解放
private:
	Vector2 pos_;			//マウスの座標
	int pageCnt_;			// ページカウンター
	int guideImage_[static_cast<int>(GuidePage::MAX)];	// ルール説明に関しては画像に文章等もまとめて表示する(ページとして何枚かに分ける)
};

