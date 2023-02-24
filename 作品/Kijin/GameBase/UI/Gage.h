#pragma once
#include "UiBase.h"
#include <functional>
#include "../Common/SharedHandle.h"

// Hp�Ƃ��X�^�~�i�̕\���Ŏg���Q�[�W
class Gage :
	public UiBase
{
	using DataBindFunc = std::function<float(ObjectManager&)>;
public:

	Gage(DataBindFunc&& dataBindFunc, const std::filesystem::path& gagePath, const std::filesystem::path& framePath,float defalutValue, const Vector2& pos);
	
private:
	void Update(float delta, ObjectManager& objMng, Controller& controller) final;
	void Draw() final;
	UiID GetUIID(void) final
	{
		return UiID::Frame;
	}
	void Loaded(void) final;

	// �Q�[�W�̉摜
	SharedGraphicHandle gageHandle_;

	// �Q�[�W�̃t���[���̉摜
	SharedGraphicHandle frameHandle_;

	// �f�[�^��R�t���鏈��
	DataBindFunc dataBindFunc_;

	// �l
	float value_;
};

