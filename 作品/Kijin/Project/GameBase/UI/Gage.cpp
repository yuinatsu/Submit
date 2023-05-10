#include "Gage.h"
#include <DxLib.h>
#include "../Common/ResourceMng.h"
#include "../SceneManager.h"
#include "../Object/ObjectManager.h"
#include "../Common/Debug.h"
#include "../Component/Transform/Transform.h"
#include "../Component/Behavior/EnemyBehavior.h"

Gage::Gage(const int& id, const DataBindFunc& dataBindFunc, const std::filesystem::path& gagePath, const std::filesystem::path& framePath, float defalutValue, const Vector2& pos) :
	UiBase{ pos }, id_{ static_cast<GageID>(id) }, dataBindFunc_{ dataBindFunc }, defValue_{ defalutValue }, value_{ defalutValue }, subGageValue_{ defalutValue }
{
	lpSceneMng.GetResourceMng().LoadTexture(gageHandle_, gagePath);
	lpSceneMng.GetResourceMng().LoadTexture(frameHandle_, framePath);
	power_ = 0.0f;
}

void Gage::Update(float delta, BaseScene& scene, ObjectManager& objMng, Controller& controller)
{
	// �e�Q�[�W�X�V
	switch (id_)
	{
	case GageID::Hp:
		HpUpdate(objMng);
		break;
	case GageID::Skill:
		SkillUpdate(objMng);
		break;
	case GageID::Stamina:
		AssignGageValue(objMng);
		break;
	default:
		break;
	}
}

void Gage::HpUpdate(ObjectManager& objMng)
{
	// �Q�[�W�ω��Ȃ��̏�Ԃ͓���Ȃ�
	if (Clamp(dataBindFunc_(objMng) / size_.x) == subGageValue_)
	{
		return;
	}
	// �����l
	power_ = objMng.GetComponent<EnemyBehavior>(objMng.GetEnemyID())->GetEnemyPower();
	// �K��l�ɓ��B������
	if (value_ <= Clamp(dataBindFunc_(objMng) / size_.x))
	{
		// �K��l���
		AssignGageValue(objMng);
		// �T�u�Q�[�W��������
		DecSubGageValue(objMng, 100.0f);
		return;
	}
	// �l��������
	FlucGageValue(objMng, 20.0f, false);
}

void Gage::SkillUpdate(ObjectManager& objMng)
{
	// �Q�[�W���Z�b�g
	if (!dataBindFunc_(objMng))
	{
		value_ = 0.0f;
	}
	// �K��l�ɓ��B������
	if (value_ >= Clamp(dataBindFunc_(objMng) / size_.x))
	{
		// �K��l���
		AssignGageValue(objMng);
		return;
	}
	// �l��������
	FlucGageValue(objMng, 50.0f, true);
}

void Gage::FlucGageValue(ObjectManager& objMng, float cnt, bool isUp)
{
	// ��������(��:1�A��:-1)
	auto fluc = isUp ? 1 : -1;
	// ���Z����
	value_ += Clamp((dataBindFunc_(objMng) / size_.x) / cnt) * fluc;
	DebugLog(dataBindFunc_(objMng));
}

void Gage::DecSubGageValue(ObjectManager& objMng, float cnt)
{
	// ��������
	subGageValue_ -= Clamp((dataBindFunc_(objMng) / size_.x) / cnt);
	// �K��l�ɓ��B������
	if (subGageValue_ <= Clamp(dataBindFunc_(objMng) / size_.x))
	{
		// �K��l���
		subGageValue_ = value_;
	}
}

void Gage::AssignGageValue(ObjectManager& objMng)
{
	// �K��l���
	value_ = Clamp(dataBindFunc_(objMng) / size_.x);
}

void Gage::Draw(int mainScr)
{
	// HP�Q�[�W�̂݃T�u�Q�[�W�\��
	if (id_ == GageID::Hp)
	{
		DrawBoxAA(pos_.x, pos_.y, pos_.x + size_.x * subGageValue_ + 2.0f, pos_.y + size_.y + 2.0f, 0xff0000, true);
	}
	// ���C���Q�[�W
	DrawRectGraphF(pos_.x, pos_.y, 2, 2, static_cast<int>(size_.x * value_ + 2.0f), static_cast<int>(size_.y + 2.0f), *gageHandle_, true);
	// �Q�[�W�t���[��
	DrawGraphF(pos_.x, pos_.y, *frameHandle_, true);
}

void Gage::Loaded(BaseScene& scene)
{
	GetGraphSizeF(*gageHandle_, &size_.x, &size_.y);
}
