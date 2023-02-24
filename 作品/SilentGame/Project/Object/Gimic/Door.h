#pragma once
#include "GimicObj.h"

class TmxObj;
class Collider;
class ObjManager;

class Door :
	public GimicObj
{
public:
	Door(int myIdx, ObjManager& objMng, std::weak_ptr< GmkEvent>& gmkEvent,std::shared_ptr<TmxObj>& tmxObj, const Math::Vector2& pos);
	~Door();
private:
	ObjID GetObjID() final { return ObjID::DoorL; }
	void Draw(const Math::Vector2& offset, DrawMng& drawMng) final;
	void Update(float dleta) final;

	/// <summary> ���Ă��鎞�̏��� </summary>
	/// <param name="delta"></param>
	void Close(float delta);

	/// <summary> �J���Ă��鎞�̏��� </summary>
	/// <param name="dleta"></param>
	void Open(float dleta);

	/// <summary> �J���Ă���r���̏��� </summary>
	/// <param name="dleta">�f���^�^�C�� </param>
	void Opening(float dleta);

	/// <summary> ���Ă���r���̏��� </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	void Closing(float delta);

	/// <summary> �����������̏��� </summary>
	/// <param name="delte"></param>
	void Lock(float delte);

	/// <summary> �����ɓ����������̏��� </summary>
	/// <param name="other"> ������������̃R���C�_�[ </param>
	void Hit(Collider& other);

	// �X�V�����̊֐��|�C���^
	void (Door::* update_)(float);


	void StartAlert(void);

	void EndAlert(void);

	// ���݂̏�ԂɂȂ��Ă���̎���
	float stateTime_;

	// �h�A�̏��
	enum class State
	{
		Close,		// ���Ă���
		Opening,	// �J���Ă���r��
		Open,		// �J���Ă���
		Closing,		// ���Ă���r��
		Lock		// 
	};

	/// <summary> �h�A�̏�Ԃ��Z�b�g���� </summary>
	/// <param name="state"> �Z�b�g������ </param>
	void SetState(Door::State state);

	

	// ���g�̃R���W�����������ԍ�
	int myIdx_;

	// ���g�̏��
	State state_;

	// tmxObj
	std::shared_ptr<TmxObj> tmxObj_;

	// lock��Ԃւ̈ȍ~�t���O
	bool lockFlag_;

	ObjManager& objMng_;
};

