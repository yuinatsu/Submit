#include <DxLib.h>
#include <string>
#include "BaseScene.h"
#include "../Input/Keyboard.h"
#include "../Input/Pad.h"

BaseScene::BaseScene()
{
	SetDrawScreen(DX_SCREEN_BACK);		// �����I�Ƀo�b�N�o�b�t�@���w��
	GetDrawScreenSize(&screenSize_.x, &screenSize_.y);	// �`���̃T�C�Y���擾����(�o�b�t�@�j
	screenID_ = MakeScreen(screenSize_.x, screenSize_.y, true);		// �X�N���[���̃T�C�Y
	// �R���g���[���[���
	if (GetJoypadNum())
	{
		controller_ = std::make_unique<Pad>();
	}
	else
	{
		controller_ = std::make_unique<Keyboard>();
	}
	mapNum_ = 1;
	extRate_ = 1.0;
	angle_ = 0;
}

BaseScene::~BaseScene()
{
	DeleteGraph(screenID_);
}

void BaseScene::Draw(float delta)
{
	// screenID�ɑ΂��ĕ`��
	DrawGraph(0, 0, screenID_, true);
}

void BaseScene::Draw(float delta, Math::Vector2 pos, double extRate)
{
	DrawRotaGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), extRate, 0, screenID_, false);
}

void BaseScene::Draw(Math::Vector2I pos, double extRate, double angle)
{
	extRate_ += extRate;
	angle_ += angle;
	auto scr = screenSize_ / 2;
	DrawRotaGraph2(scr.x + pos.x, scr.y + pos.y, scr.x + pos.x, scr.y + pos.y, extRate_, angle_, screenID_, true);
}

void BaseScene::SoundPlay(void)
{
}

void BaseScene::Loaded(void)
{
}
