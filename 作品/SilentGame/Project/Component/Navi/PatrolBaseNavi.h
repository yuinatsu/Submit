#pragma once
#include <memory>
#include <vector>
#include "../Component.h"
#include "../../common/Math.h"

class TmxObj;

class PatrolBaseNavi :
	public Component
{
	using Vector2lVec = std::vector<Math::Vector2I>;
public:
	PatrolBaseNavi(Vector2lVec& route, std::shared_ptr<TmxObj>& tmxObj, Obj& owner);
	virtual ~PatrolBaseNavi();

	/// <summary> ���s���ׂ����W���擾���� </summary>
	/// <param name=""></param>
	/// <returns> ���W </returns>
	virtual const Math::Vector2 GetMovePos(void) const = 0;

	/// <summary> ���݂̃}�X�� </summary>
	/// <param name=""></param>
	/// <returns></returns>
	virtual const Math::Vector2I& GetPos(void)const = 0;

	/// <summary> ��~���Ă��邩 </summary>
	/// <param name=""></param>
	/// <returns> ��~���Ă���Ƃ�true���Ă��Ȃ��Ƃ�false </returns>
	bool IsStop(void) const
	{
		return stopFlag_;
	}

	/// <summary> ��~������ </summary>
	/// <param name=""></param>
	void Stop(void)
	{
		stopFlag_ = true;
	}

	/// <summary> ������J�n���� </summary>
	/// <param name=""></param>
	void Start(void)
	{
		stopFlag_ = false;
	}

protected:
	

	ComponentID GetID() const
	{
		return ComponentID::PatrolNavi;
	}


	// �}�b�v���
	std::shared_ptr<TmxObj> tmxObj_;


	// ����p���[�g
	Vector2lVec route_;

	
	// ��~�t���O
	bool stopFlag_;
};

