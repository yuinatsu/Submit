#pragma once
#include "Behavior.h"
#include "../ComponentHandle.h"
#include "../Render/EffectRender.h"

// 一部エフェクトを再生終了と同時に消滅させたり制御するクラス
class EffectBehavior :
	public Behavior
{
public:


	ComponentID GetID(void) const override
	{
		return id_;
	}
	static constexpr ComponentID id_{ ComponentID::EffectBehavior };
private:
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;
	void Begin(ObjectManager& objectManager) final;

	// エフェクトの描画クラス
	ComponentHandle<EffectRender> effectRender_;
};

