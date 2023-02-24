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
	//// �\���̂��g�p���ɂ���
	//info_->usingEffect_ = true;

	//// �I���̃��N�G�X�g������Ă��邩�ǂ���
	//info_->endRequest_ = false;

	//// �G�t�F�N�g�̓��e�ۑ�
	//info_->effect_ = index_;

	////�G�t�F�N�g�̊�{���̃A�h���X���Z�b�g
	//info_->imageInit_ = &baseInit_[index_];

	////�G�t�F�N�g���Ƃ̃f�[�^���i�[���Ă���̈�̃A�h���X�ۑ��p�|�C���^�̏�����
	//info_->subDate_ = NULL;

	//// �G�t�F�N�g�ʂ̍쐬���ɌĂԊ֐�
	////if()

	////�I��

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

	










