#pragma once
#include "BaseScene.h"

// �ݒ��ʓ��̃V�[��
class OptionScene :
    public BaseScene
{
public:
	OptionScene();

private:
	const SceneID GetID(void) const final
	{
		return SceneID::Option;
	}

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="delta"></param>
	void Update(float delta, Controller& controller) final;

	/// <summary>
	/// �V�[���̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawScene(void) final;

};

