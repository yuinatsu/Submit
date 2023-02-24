#include <DxLib.h>
#include "BlendTrans.h"
#include "../../common/Debug.h"

BlendTrans::BlendTrans(double limitTime, uniqueScene beforScene, uniqueScene afterScene) :
	TransitionScene(std::move(beforScene), std::move(afterScene))
{
	limitTime_ = limitTime;
	addExt_ = 0.01f;
	DrawOwnScreen(0.0);
}

BlendTrans::~BlendTrans()
{
}

bool BlendTrans::UpdataTransition(double delta)
{
	count += delta;
	// limitTime秒経ったら次のシーンへ以降
	// トランジションが終わっている＆画面回転が360度の時点でtrue
	if (count > limitTime_ && (((float)((int)(afterScene_->GetAngle() * 10) % 63) / 10.0f) == 0))
	{
		return true;
	}
	// 終わっていなければfalse
	return false;
}

void BlendTrans::DrawOwnScreen(float delta)
{
	// 自分自身のスクリーンに対してDraw
	SetDrawScreen(screenID_);
	//SetTransColor(255, 255, 255);
	GraphFilter(screenID_, DX_GRAPH_FILTER_GAUSS, 8, 30);
	auto alpha = static_cast<int>(255 * count / limitTime_);
	// 透明にする計算
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	beforScene_->Draw(Math::Vector2I(200,0), addExt_, -0.05);
	// 色をつける
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	//afterScene_->Draw(addExt_, 0.1f);
	// ずっと有効になってしまうので無効にする命令
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
