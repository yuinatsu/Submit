#include <DxLib.h>
#include <algorithm>
#include "DrawMng.h"
#include "../Scene/SceneMng.h"
#include "Debug.h"

constexpr float posMin{ -16.0f };

// �͈̓`�F�b�N����
constexpr bool CheckDrawPos(const Math::Vector2& pos)
{
	// �͈͊O�̎�true
	return pos.x < posMin || pos.x > static_cast<float>(lpSceneMng.GetScreenSize().x) || pos.y < posMin || pos.y > static_cast<float>(lpSceneMng.GetScreenSize().y);
}

DrawMng::DrawMng(float maxPosY) :
	maxPosY_{maxPosY}
{
	queueCount_ = 0;
}

DrawMng::~DrawMng()
{
}

void DrawMng::Draw(const Math::Vector2& pos, int handle)
{
	if (CheckDrawPos(pos))
	{
		// �͈͊O�̎��`���ǉ����Ȃ�
		return;
	}
	Draw([this, pos, handle]() { DrawGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), handle, true); }, GetPosY2Depth(pos.y));
}

void DrawMng::Draw(const Math::Vector2& pos, float rot, int handle)
{
	if (CheckDrawPos(pos))
	{
		// �͈͊O�̎��`���ǉ����Ȃ�
		return;
	}
	Draw([this, pos, rot, handle]() { DrawRotaGraph(static_cast<int>(pos.x), static_cast<int>(pos.y),1.0, rot, handle, true); }, GetPosY2Depth(pos.y));
}

void DrawMng::Draw(const Math::Vector2& pos, float depth, float rot, int handle)
{
	// �`��͈̓`�F�b�N
	if (CheckDrawPos(pos))
	{
		// �͈͊O�̎��`���ǉ����Ȃ�
		return;
	}

	Draw([this, pos, rot,handle]() { DrawRotaGraph(static_cast<int>(pos.x), static_cast<int>(pos.y), 1.0, rot, handle, true); }, depth);
}

void DrawMng::Draw(void)
{
	// �I�[�ɓ�����C�e���[�^�[���擾
	auto end = (drawQueue_.begin() + queueCount_);

	// �[�x�̑傫�����Ƀ\�[�g
	std::sort(drawQueue_.begin(), end, [](auto& a, auto& b) {return a.second > b.second; });

	// ���Ԃɕ`�悷��(�l�I�ɉ��ɂ�����̂���)
	for (int i = 0; i < queueCount_; i++)
	{
		drawQueue_[i].first();
	}

	// �L���[��clear
	queueCount_ = 0;
}

void DrawMng::Draw(std::function<void(void)>&& func, float depth)
{
	// �[�x���N�����v����
	depth = Math::Clamp(depth, 0.0f, 1.0f);

	// queue�̃`�F�b�N
	if (queueCount_ < drawQueue_.size())
	{
		// queue�̐����ő�T�C�Y��菬�����Ƃ����ԂɃf�[�^������
		drawQueue_[queueCount_] = std::make_pair(
			func,
			depth
		);
	}
	else
	{
		// �����łȂ��Ƃ���납��V���ɒǉ����Ă���
		drawQueue_.emplace_back(
			std::make_pair(
				func,
				depth
			)
		);
	}
	queueCount_++;
}
