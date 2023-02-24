#pragma once
#include "Unit.h"

// �Q�[���N���A�ڕW�ɓ��������
class Target :
	public Unit
{
public:
	Target(std::vector<Math::Vector2I>& route,std::shared_ptr<TmxObj>& tmxObj,const Math::Vector2& pos);

	/// <summary> �N���A�������H </summary>
	/// <param name=""> true�̎��N���A </param>
	/// <returns></returns>
	const bool IsClear(void)const { return isClear_; }
private:

	void Draw(const Math::Vector2& offset, DrawMng& drawMng)override;
	ObjID GetObjID(void) override { return ObjID::Target;}

	/// <summary> �ړ���������A�j���[�V������؂�ւ��� </summary>
	/// <param name=""></param>
	void CorrectMoveVecToAnim(void) final;

	// �N���A�t���O
	bool isClear_;
};

