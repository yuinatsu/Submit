#pragma once
#include "UiBase.h"
#include <functional>
#include "../Common/SharedHandle.h"

// Hpとかスタミナの表示で使うゲージ
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

	// ゲージの画像
	SharedGraphicHandle gageHandle_;

	// ゲージのフレームの画像
	SharedGraphicHandle frameHandle_;

	// データを紐付ける処理
	DataBindFunc dataBindFunc_;

	// 値
	float value_;
};

