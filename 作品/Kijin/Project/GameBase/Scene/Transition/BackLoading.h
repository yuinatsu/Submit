#pragma once
#include "Loading.h"

// �w�i�ɑJ�ڑO�̃V�[����`�悷��N���X
class BackLoading :
    public Loading
{
public:
	BackLoading(SceneUptr befor, SceneUptr after, float transTime);
private:
	/// <summary>
	/// �A�b�v�f�[�g����
	/// </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	/// <param name="controller"> �R���g���[���[ </param>
	void Update(float delta, Controller& controller) final;

	/// <summary>
	/// �V�[���̕`��
	/// </summary>
	/// <param name=""></param>
	void DrawScene(void) final;

};

