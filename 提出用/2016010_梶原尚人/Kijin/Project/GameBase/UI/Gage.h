#pragma once
#include "UiBase.h"
#include <functional>
#include "../Common/SharedHandle.h"

enum class GageID
{
	Hp,
	Skill,
	Stamina,
	Max
};

// Hp�Ƃ��X�^�~�i�̕\���Ŏg���Q�[�W
class Gage :
	public UiBase
{
	using DataBindFunc = std::function<float(ObjectManager&)>;
public:

	Gage(const int& id, const DataBindFunc& dataBindFunc, const std::filesystem::path& gagePath, const std::filesystem::path& framePath,float defalutValue, const Vector2& pos);
	
private:
	void Update(float delta, BaseScene& scene, ObjectManager& objMng, Controller& controller) final;
	void HpUpdate(ObjectManager& objMng);
	void SkillUpdate(ObjectManager& objMng);
	void FlucGageValue(ObjectManager& objMng, float cnt, bool isUp);
	void DecSubGageValue(ObjectManager& objMng, float cnt);
	void AssignGageValue(ObjectManager& objMng);
	void Draw(int mainScr) final;
	UiID GetUIID(void) final
	{
		return UiID::Frame;
	}
	void Loaded(BaseScene& scene) final;

	// �Q�[�W�̉摜
	SharedGraphicHandle gageHandle_;

	// �Q�[�W�̃t���[���̉摜
	SharedGraphicHandle frameHandle_;

	// �f�[�^��R�t���鏈��
	DataBindFunc dataBindFunc_;

	// �l
	float value_;

	// �Q�[�WID
	GageID id_;

	// �f�t�H���g�̃Q�[�W�l
	float defValue_;
	// �Q�[�W�����O�̒l
	float subGageValue_;

	// �����l
	float power_;
};

