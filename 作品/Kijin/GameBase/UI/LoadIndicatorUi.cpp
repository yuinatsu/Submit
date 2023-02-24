#include <DxLib.h>
#include "LoadIndicatorUi.h"
#include "../Application.h"
#include "../Common/ResourceMng.h"
#include "../Common/Math.h"
#include "../Common/Debug.h"

LoadIndicatorUi::LoadIndicatorUi(const std::filesystem::path& path, const Vector2& pos) : UiBase(pos)
{
	lpResourceMng.LoadTexture(handle_, path);
	pos_ = pos;
	GetGraphSizeF(*handle_, &size_.x, &size_.y);
	// �`��p�x������
	angle_ = Deg2Rad(0.0f);
	time_ = 0.0f;
	cnt_ = 0;
}

LoadIndicatorUi::~LoadIndicatorUi()
{
}

void LoadIndicatorUi::Update(float delta, ObjectManager& objMng, Controller& controller)
{
	cnt_++;
	if (time_ <= (delta * 3.0f))
	{
		time_ += delta;
		return;
	}
	time_ = 0.0f;
	angle_ += Deg2Rad(360.0f / 12.0f);
}

void LoadIndicatorUi::Draw()
{
	// �C���W�P�[�^�[�`��
	DrawRotaGraph(pos_.x,pos_.y, 1.0, angle_, *handle_, true, false);
	// Now Loading�`��
	DrawString(pos_.x - 55.0f, pos_.y + 70.0f, L"Now Loading", 0xffffff);
	// Now Loading�̌��...�̃A�j���[�V�����`��
	auto num = (cnt_ / 20) % 3 + 1;
	auto interval = 5;
	for (int i = 1; i <= num; i++)
	{
		DrawString(pos_.x + 40 + interval, pos_.y + 70.0f, L".", 0xffffff);
		interval += 5;
	}
}
