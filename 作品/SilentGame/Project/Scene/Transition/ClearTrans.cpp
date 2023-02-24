#include <DxLib.h>
#include "ClearTrans.h"
#include "../SceneMng.h"
#include "../../common/Debug.h"
#include "../../Object/ObjManager.h"
#include "../../common/Camera/Camera.h"
#include "../../Resource/Tmx/TmxObj.h"
#include "../..//common/GmkEvent.h"
#include "../../common/ImageMng.h"

ClearTrans::ClearTrans(std::shared_ptr<ObjManager> objMng, DrawMng& drawMng, Camera& camera, std::shared_ptr<TmxObj> tmxObj, std::shared_ptr<GmkEvent> gEvent, uniqueScene beforScene, uniqueScene afterScene) : TransitionScene(std::move(beforScene), std::move(afterScene)), drawmng_(drawMng), camera_(camera), gEvent_(gEvent)
{
	objMng_ = objMng;
	tmxObj_ = tmxObj;
	// ファンクション初期化
	updateFunc_ = std::bind(&ClearTrans::UpdateZoom, this, std::placeholders::_1);
	drawFunc_ = std::bind(&ClearTrans::DrawZoom, this, std::placeholders::_1);
	limitTime_ = 5.0;
	cameraSize_ = lpSceneMng.GetCameraSize();
	offsetSize_ = (Math::Vector2)screenSize_ - cameraSize_;
	DrawOwnScreen(0.0);
	zoomExt_ = 1.0;
	zoomPos_ = Math::Vector2(0,0);
	viewID_ = MakeScreen(camera_.GetViewSize().x, camera_.GetViewSize().y, 255);
	effectF_ = false;

	auto camPos = camera_.GetPos() - cameraSize_ / 2.0f;
	auto pPos = objMng_->GetPlayer().lock()->GetPos() - camPos;
	distance_ = pPos - cameraSize_ / 2.0f;
}

ClearTrans::~ClearTrans()
{
}

bool ClearTrans::UpdataTransition(double delta)
{
	updateFunc_(delta);
	return isEnd_;
}

void ClearTrans::UpdateZoom(double delta)
{
	if (zoomExt_ < 2.0f)
	{
		zoomExt_ += delta;
	}
	if ((Math::Vector2I)distance_ >= (-((Math::Vector2I)zoomPos_) - 2) && (Math::Vector2I)distance_ <= (-((Math::Vector2I)zoomPos_) + 2))
	{
		effectF_ = true;
		updateFunc_ = std::bind(&ClearTrans::UpdateSlash, this, std::placeholders::_1);
		drawFunc_ = std::bind(&ClearTrans::DrawSlash, this, std::placeholders::_1);
	}
	else
	{
		Math::Vector2 interval = distance_ / 10.0f;
		zoomPos_ -= interval;
	}
	camera_.Update((float)delta);
}

void ClearTrans::UpdateSlash(double delta)
{
	count += delta;
	// 0.5秒後に斬撃アニメーション
	if (count >= 0.5 && effectF_)
	{
		objMng_->AddEffect(EffectNameWhole::Super, screenSize_ / 2);
		effectF_ = false;
	}
	// 1.5秒後に爆破アニメーション
	else if (count >= 2.5)
	{
		count = 0;
		updateFunc_ = std::bind(&ClearTrans::UpdateExplosion, this, std::placeholders::_1);
		drawFunc_ = std::bind(&ClearTrans::DrawExplosion, this, std::placeholders::_1);
		gEvent_->StartAlert();
	}
	// エフェクト更新
	objMng_->UpdateEffect((float)delta);
	camera_.Update((float)delta);
}

void ClearTrans::UpdateExplosion(double delta)
{
	count += delta;
	// limitTime秒後に次のシーンに行く
	if (count > limitTime_)
	{
		gEvent_->EndAlert();
		isEnd_ = true;
	}
	if (zoomExt_ > 1.1)
	{
		zoomExt_ -= (delta / 5);
		auto pp = 100.0f;
		Math::Vector2 interval = ((distance_ * pp) * (float)(delta * pp) / (pp * pp));
		zoomPos_ += interval / 5.0f;
	}
	gEvent_->Update((float)delta);
	camera_.Update((float)delta);
}

void ClearTrans::DrawOwnScreen(float delta)
{
	drawFunc_(delta);
}

void ClearTrans::DrawZoom(double delta)
{
	SetDrawScreen(viewID_);
	ClsDrawScreen();
	beforScene_->Draw((float)delta, (Math::Vector2)screenSize_ / 2.0f + zoomPos_, zoomExt_);
	camera_.Draw();

	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(50, 60, viewID_, true);
}

void ClearTrans::DrawSlash(double delta)
{
	SetDrawScreen(viewID_);
	ClsDrawScreen();
	beforScene_->Draw((float)delta, (Math::Vector2)screenSize_ / 2.0f + zoomPos_, zoomExt_);
	camera_.Draw();

	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(50, 60, viewID_, true);
	// エフェクト描画
	objMng_->EffectDraw(drawmng_);
	drawmng_.Draw();
}

void ClearTrans::DrawExplosion(double delta)
{
	SetDrawScreen(viewID_);
	ClsDrawScreen();
	auto alpha = static_cast<int>(255 * count / limitTime_);
	// 透明にする計算
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	beforScene_->Draw((float)delta, (Math::Vector2)screenSize_ / 2.0f + zoomPos_, zoomExt_);
	// エフェクト描画
	objMng_->EffectDraw(drawmng_);
	drawmng_.Draw();
	// ずっと有効になってしまうので無効にする命令
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);
	gEvent_->Draw();
	camera_.Draw();

	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(50, 60, viewID_, true);
}
