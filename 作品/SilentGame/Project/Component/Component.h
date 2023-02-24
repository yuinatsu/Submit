#pragma once
#include <string>
#include "../common/Math.h"
#include "ComponentID.h"
class Obj;

/// <summary> �I�u�W�F�N�g�ɋ@�\����������p�̊��N���X </summary>
class Component
{
public:

	/// <summary> �R���X�g���N�^ </summary>
	/// <param name="owner"> ������̃I�u�W�F�N�g </param>
	Component(Obj& owner);

	/// <summary> �f�X�g���N�^ </summary>
	virtual ~Component();

	/// <summary> �X�V���� </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	virtual void Update(float delta);

	virtual void Update(float delta, std::string key);

	virtual ComponentID GetID() const = 0;
#ifdef _DEBUG
	// �f�o�b�O�p�`��
	virtual void DbgDraw(const Math::Vector2& offset) const {}
#endif
protected:

	/// <summary> ������ </summary>
	Obj& owner_;

};

