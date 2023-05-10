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
	Render();
	virtual ~Render();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="shadowMap"></param>
	/// <param name="buff"></param>
	virtual void Draw(int shadowMap = -1,int buff = -1) = 0;

	/// <summary>
	/// �V���h�E�}�b�v�쐬�p
	/// </summary>
	/// <param name=""></param>
	virtual void SetUpDepthTex(int ps = -1,int buff = -1) = 0;
	
	/// <summary>
	/// �ǂݍ���
	/// </summary>
	/// <param name="path"></param>
	/// <param name="scene"></param>
	virtual void Load(const std::filesystem::path& path) = 0;

	void Begin(ObjectManager& objectManager) override;

	void SetDrawFlag(bool isDraw)
	{
		isDraw_ = isDraw;
	}

	ComponentID GetID(void) const override
	{
		return id_;
	}

	SetID(ComponentID::RenderBase, ComponentID::RenderBase)

protected:

	// �g�����X�t�H�[��
	ComponentHandle<Transform> transform_;

	// �`�悷�邩�̃t���O
	bool isDraw_;
};

