#pragma once
#include <functional>
#include "BehaviorBaseNode.h"

using ActFunc = std::function<NodeState(Obj&, float)>;

// �A�N�V�����m�[�h(���[�ŉ��������s����)
class ActionNode :
	public BehaviorBaseNode
{
public:
	ActionNode(const ActFunc& actFunc);
	~ActionNode();
private:
	// ���s����֐�
	ActFunc actFunc_;

	/// <summary> �����Ă���function�����s���� </summary>
	/// <param name="obj"> obj </param>
	/// <param name="delta"> �f���^�^�C�� </param>
	/// <returns></returns>
	NodeState operator()(Obj& obj, float delta) override;

	/// <summary> ���M���R�s�[���� </summary>
	/// <param name=""></param>
	/// <returns></returns>
	std::unique_ptr<BehaviorBaseNode> Copy(void) override;
};

