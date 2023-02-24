#pragma once
#include "TransitionType.h"
#include "../BaseScene.h"

class Loading :
	public BaseScene
{
public:
	Loading(SceneUptr loadScene, TransitionType type, float transTime, bool isIndicator = true);

private:
	const SceneID GetID(void) const final
	{
		return SceneID::Loading;
	}
	void Update(float delta, Controller& controller) final;
	void LoadingIndicator(float delta);
	void Relese(void) final;
	void DrawScene(void) final;
	SceneUptr MakeNextFunc(SceneUptr own);

	TransitionType type_;

	// ���[�h����V�[��
	SceneUptr loadScene_;

	const float transTime_;
	
	// ���[�h���̃A�C�R���Ȃǂ�\�����邩
	bool isIndicator_;
};