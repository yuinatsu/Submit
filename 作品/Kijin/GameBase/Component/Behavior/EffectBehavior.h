#pragma once
#include "Behavior.h"
#include "../ComponentHandle.h"
#include "../Render/EffectRender.h"

// �ꕔ�G�t�F�N�g���Đ��I���Ɠ����ɏ��ł������萧�䂷��N���X
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

	// �G�t�F�N�g�̕`��N���X
	ComponentHandle<EffectRender> effectRender_;
};

