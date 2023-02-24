#pragma once
#include "../ComponentBase.h"
#include <filesystem>
#include "../Transform/Transform.h"
#include "../ComponentHandle.h"

class BaseScene;

// �`��p�̃N���X
class Render :
	public ComponentBase
{
public:

	virtual ~Render();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name=""></param>
	virtual void Draw(void) = 0;

	virtual void Draw(int shadowMap,int buff) = 0;

	/// <summary>
	/// �V���h�E�}�b�v�쐬�p
	/// </summary>
	/// <param name=""></param>
	virtual void SetUpShadowMap(void) = 0;
	
	/// <summary>
	/// �ǂݍ���
	/// </summary>
	/// <param name="path"></param>
	/// <param name="scene"></param>
	virtual void Load(const std::filesystem::path& path) = 0;

	void Begin(ObjectManager& objectManager) override;

	ComponentID GetID(void) const override
	{
		return id_;
	}

	SetID(ComponentID::RenderBase, ComponentID::RenderBase)

protected:
	ComponentHandle<Transform> transform_;
};

