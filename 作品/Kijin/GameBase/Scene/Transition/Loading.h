#pragma once
#include "../BaseScene.h"

class Loading :
	public BaseScene
{
public:
	Loading(SceneUptr befor, SceneUptr after, float transTime);
	virtual ~Loading();
protected:

	// ���[�h����V�[��
	SceneUptr after_;

	// �J�ڌ��̃V�[��
	SceneUptr befor_;

	// �J�ڎ���
	const float transTime_;

	// �o�ߎ���
	float stepTime_;


private:
	const SceneID GetID(void) const final
	{
		return SceneID::Loading;
	}
	
	void LoadingIndicator(float delta);
	void Relese(void) final;
	SceneUptr MakeNextFunc(SceneUptr own);
	

	// ���[�h���̃A�C�R���Ȃǂ�\�����邩
	bool isIndicator_;

	


};