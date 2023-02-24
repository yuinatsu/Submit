#include <DxLib.h>
#include "DeadTrans.h"
#include "../SceneMng.h"
#include "../../common/Debug.h"
#include "../../Object/ObjManager.h"

DeadTrans::DeadTrans(std::shared_ptr<ObjManager> objMng, Math::Vector2 offset, DrawMng& drawMng, Camera& camera, uniqueScene beforScene, uniqueScene afterScene) : TransitionScene(std::move(beforScene), std::move(afterScene)), drawmng_(drawMng), camera_(camera)
{
	objMng_ = objMng;
	offset_ = offset;
	limitTime_ = 5.0;
	// �J�����T�C�Y
	cameraSize_ = lpSceneMng.GetCameraSize();
	offsetSize_ = (Math::Vector2)screenSize_ - cameraSize_;
	viewID_ = MakeScreen(camera_.GetViewSize().x, camera_.GetViewSize().y, 255);
	// �G�t�F�N�g�ǉ�
	objMng_->AddEffect(EffectName::Slash, objMng_->GetPlayer().lock()->GetPos() + offsetSize_ / Math::Vector2(2, 2));
	objMng_->AddEffect(EffectName::Explosion, objMng_->GetPlayer().lock()->GetPos() + offsetSize_ / Math::Vector2(2, 2));
	DrawOwnScreen(0.0);
}

DeadTrans::~DeadTrans()
{
}

bool DeadTrans::UpdataTransition(double delta)
{
	count += delta;
	// �G�t�F�N�g�X�V
	objMng_->UpdateEffect((float)delta);
	// limitTime�b�o�߂����玟�̃V�[���ɍs��
	if (count > limitTime_)
	{
		return true;
	}
	return false;
}

void DeadTrans::DrawOwnScreen(float delta)
{
	SetDrawScreen(viewID_);
	ClsDrawScreen();
	auto alpha = static_cast<int>(255 * count / limitTime_);
	// �����ɂ���v�Z
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	beforScene_->Draw(delta);
	// �F������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	afterScene_->Draw(delta);
	// �����ƗL���ɂȂ��Ă��܂��̂Ŗ����ɂ��閽��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, alpha);

	SetDrawScreen(screenID_);
	ClsDrawScreen();
	DrawGraph(50, 60, viewID_, true);
	// �G�t�F�N�g�`��
	objMng_->EffectDraw(offset_, drawmng_);
	drawmng_.Draw();
}
