#include <DxLib.h>
#include "Effect.h"
#include "../../common/AnimMng.h"
#include "../../Component/Animation.h"
#include "../../common/nameof.hpp"
#include "../../common/Debug.h"


Effect::Effect(EffectName effect, const Math::Vector2& pos) :
	Obj{pos}
{
	// �A�j���[�V�����N���X�ǉ�
	AddComponent(std::make_unique<Animation>("Resource/Xml/EffectImage.xml", "Effect", std::string(NAMEOF_ENUM(effect)), *this));
}

Effect::Effect(EffectName_x120 effect, const Math::Vector2& pos) :
	Obj{pos}
{
	// �A�j���[�V�����N���X�ǉ�
	AddComponent(std::make_unique<Animation>("Resource/Xml/EffectImage_x120.xml", "Effect_x120", std::string(NAMEOF_ENUM(effect)), *this));
}

Effect::Effect(EffectNameWhole effect, const Math::Vector2& pos) :
	Obj{ pos }
{
	// �A�j���[�V�����N���X�ǉ�
	AddComponent(std::make_unique<Animation>("Resource/Xml/EffectWholeImage.xml", "EffectWhole", std::string(NAMEOF_ENUM(effect)), *this));
}

Effect::~Effect()
{
}

void Effect::Update(float delta)
{
	// �A�j���[�V�����X�V
	UpdateComponent(delta);
	// �Đ���ɏ���
	if (dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).IsEnd())
	{
		Destroy();
		isAtk_ = false;
	}
}

void Effect::Draw(const Math::Vector2& offset, DrawMng& drawMng)
{
	// �G�t�F�N�g�\��
	dynamic_cast<Animation&>(GetComponent(ComponentID::Animator)).Draw(drawMng, pos_ + offset, false);
}
