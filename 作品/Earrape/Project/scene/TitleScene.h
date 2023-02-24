#pragma once
#include "BaseScene.h"
#include "../SCN_ID.h"
#include "../common/Vector2.h"

class TitleScene :
    public BaseScene
{
public:
	TitleScene(){
		titleLogoBO_ = titleLogoBW_ = titleGuideLogoBO_ = titleGuideLogoBW_ = section1_BO_ = section1_BW_ = section2_BO_ = section2_BW_ = section3_BO_ = section3_BW_ = titlebg_ = 0;
		atfSE_ = okSE_ = 0;
		sctFlag1_ = sctFlag2_ = sctFlag3_ = false;
	}
	~TitleScene() {};
	SCN_ID Update(char* keyData, char* keyDataOld) override;
	void Draw(void) override;	// グラフィックの描画
	bool Init(void) override;	// 初期化
	bool Release(void) override;	// リソースの解放

private:
	int titleLogoBO_, titleLogoBW_, titleGuideLogoBO_, titleGuideLogoBW_, section1_BO_, section1_BW_, section2_BO_, section2_BW_, section3_BO_, section3_BW_, titlebg_;
	int atfSE_, okSE_;
	bool sctFlag1_, sctFlag2_, sctFlag3_;

	Vector2 pos_;
};
