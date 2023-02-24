#include <DxLib.h>
#include "EffekseerForDXLib.h"
#include "../../Common/ResourceMng.h"
#include "../../Application.h"
#include "../../Object/ObjectManager.h"
#include "EffectRender.h"
//
EffectRender::EffectRender()
{
}

EffectRender::~EffectRender()
{

}

bool EffectRender::IsEnd(void)
{
	if (isEnd_ == true) {
		return true;
	}
	return false;
}

bool EffectRender::Init(void)
{
	/*int num_;
	EffectInfo *info_;
	EffectInit *effectInit;*/

	//effectInit = baseInit_[EffectMax];

	return false;
}

void EffectRender::Load(const std::filesystem::path& path)
{
	
	lpResourceMng.LoadEffect(handle_, path);

	LoadEffect();
}

void EffectRender::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	if (transform_.IsActive()) {
		UpdateEffekseer();
	}
	//{
}

void EffectRender::Draw(void)
{
	DrawEffekseer3D();
	effectTime_++;


	
}
//
//bool EffectRender::EffectEndRequest(EffectInfo* info)
//{
//
//
//	return false;
//}
//
//EffectInfo* EffectRender::EffectCreate(EffectIndex index_)
//{
	//int num;
	//EffectInfo* info_;
	//info_ = effectInfo_;
	//for (num = 0; num < EFFECT_MAXNUM; num++, info_++)
	//{
	//	if (!info_->usingEffect_)
	//	{
	//		break;
	//	}
	//}
	//if (num == EFFECT_MAXNUM)
	//{
	//	return NULL;
	//}
	//// 構造体を使用中にする
	//info_->usingEffect_ = true;

	//// 終了のリクエストがされているかどうか
	//info_->endRequest_ = false;

	//// エフェクトの内容保存
	//info_->effect_ = index_;

	////エフェクトの基本情報のアドレスをセット
	//info_->imageInit_ = &baseInit_[index_];

	////エフェクトごとのデータを格納している領域のアドレス保存用ポインタの初期化
	//info_->subDate_ = NULL;

	//// エフェクト別の作成時に呼ぶ関数
	////if()

	////終了

	//return info_;
//}

void EffectRender::EffectDeleate(void)
{
}

void EffectRender::EffectDeleateAll(void)
{
}

void EffectRender::EffectStepAll(float stepTime)
{
}

void EffectRender::UpdateEffekseer(void)
{

	playPos_ = transform_.Get()->GetPos();
	if (effectTime_ <= effectTimeMax_.desEx) {
		playingHandle_ = PlayEffekseer3DEffect(effectResourceHandle);
	}
	else
	{
		isEnd_ = true;
	}
	SetPosPlayingEffekseer3DEffect(playingHandle_, playPos_.x, playPos_.y, playPos_.z);

	
	UpdateEffekseer3D();
}
void EffectRender::LoadEffect(void)
{
	effectResourceHandle = LoadEffekseerEffect("Resource/Effect/FireEx.efkefc", 1.0f);

}

	










