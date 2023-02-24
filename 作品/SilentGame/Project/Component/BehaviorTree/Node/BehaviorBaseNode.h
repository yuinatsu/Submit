#pragma once
#include <memory>
#include "../NodeState.h"

class Obj;

// �r�w�C�r�A�x�[�XAI�p�̃m�[�h�̊��N���X
class BehaviorBaseNode
{
public:
	BehaviorBaseNode();
	virtual ~BehaviorBaseNode();

	/// <summary> ���Z�b�g������������ </summary>
	/// <param name=""></param>
	virtual void Reset(void);

	/// <summary> �X�V���� </summary>
	/// <param name="obj"> ����Ώۂ̃I�u�W�F�N�g </param>
	/// <param name="delta"> �f���^�^�C�� </param>
	/// <returns> �m�[�h�̏�� </returns>
	virtual NodeState operator()(Obj& obj, float delta) = 0;

	/// <summary> ���M���R�s�[���� </summary>
	/// <param name=""></param>
	/// <returns></returns>
	virtual std::unique_ptr<BehaviorBaseNode> Copy(void) = 0;
};

