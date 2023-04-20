#pragma once
#include "Behavior.h"
#include "../ComponentHandle.h"
#include "../Render/EffectRender.h"

class BeemEffectBehavior :
    public Behavior
{
public:
	BeemEffectBehavior();

	void Start(void);

	ComponentID GetID(void) const override
	{
		return id_;
	}
	static constexpr ComponentID id_{ ComponentID::BeemEffectBehavior };
private:
	void Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller) final;
	void Begin(ObjectManager& objectManager) final;
	void Destory(ObjectManager& objManager) final;

	// �G�t�F�N�g�̕`��N���X
	ComponentHandle<EffectRender> effectRender_;

	// �g�����X�t�H�[��
	ComponentHandle<Transform> transform_;

	// 
	ComponentHandle<Transform> playerT_;

};

