#pragma once
#include <list>
#include <memory>
#include "../BehaviorBaseNode.h"

using NodeList = std::list<std::shared_ptr<BehaviorBaseNode>>;
using NodeListItr = NodeList::iterator;

// �Z���N�^�[or�V�[�N�G���X�m�[�h�Ƃ��̊��N���X
class ParentNode :
	public BehaviorBaseNode
{
public:
	ParentNode();
	virtual ~ParentNode();

	/// <summary> �q�m�[�h��ǉ����� </summary>
	/// <param name="child"> �ǉ��������m�[�h </param>
	/// <returns> ������true���s��false </returns>
	bool PushChildren(std::unique_ptr<BehaviorBaseNode>&& child);

protected:

	/// <summary> �ēx���[�g�����蒼�����ɌĂ� </summary>
	/// <param name=""></param>
	void Reset(void) override;

	// �q�m�[�h���X�g
	NodeList children_;

	// ���ݏ������̎q�m�[�h
	NodeListItr nowItr_;
};

