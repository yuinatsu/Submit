#include "ResultDraw.h"

constexpr float aTime{ 0.5f };

ResultDraw::ResultDraw(float startTime, const Math::Vector2& pos) :
	drawStartTime_{ startTime },pos_{pos}, drawTime_{0.0f}
{
	// �`����������Ȃ������_������Ƃ�
	drawFunc_ = []() {};
	updateFunc_ = std::bind(&ResultDraw::UpdateNon, this, std::placeholders::_1);
}

ResultDraw::~ResultDraw()
{
}

void ResultDraw::Update(const float& nowTime)
{
	updateFunc_(nowTime);
}

void ResultDraw::Draw(void)
{
	drawFunc_();
}

void ResultDraw::UpdateNon(const float& nowTime)
{
	if (nowTime >= drawStartTime_)
	{
		// �`��֐���؂�ւ�
		drawFunc_ = std::bind(&ResultDraw::DrawOwn, this);

		// �A�b�v�f�[�g�֐���؂�ւ�
		updateFunc_ = std::bind(&ResultDraw::UpdateDraw,this,std::placeholders::_1);
	}
}

void ResultDraw::UpdateDraw(const float& nowTime)
{
	// ���݂܂ł̌o�ߎ��Ԃ���`��J�n�̎��Ԃ������`�掞����̌o�ߎ��Ԃ����߂�
	drawTime_ = (nowTime - drawStartTime_);
}
