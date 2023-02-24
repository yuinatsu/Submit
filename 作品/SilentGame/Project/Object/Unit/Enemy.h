#pragma once
#include <memory>
#include "../../common/Math.h"
#include "../../common/Random.h"
#include "EnemyState.h"
#include "Unit.h"

class  Camera;
struct Fan;
class ObjManager;
class TmxObj;
class GmkEvent;
class Collider;

// �G�N���X
class Enemy :
	public Unit
{
public:
	Enemy(ObjManager& objMng, std::weak_ptr< GmkEvent>&,std::vector<Math::Vector2I>& route,std::shared_ptr<TmxObj>& tmxObj, const Math::Vector2& pos);
	~Enemy();

	/// <summary> �G�̏�Ԃ��擾 </summary>
	/// <param name=""></param>
	/// <returns> �G�̏�� </returns>
	EnemyState GetEnemyState(void) const;

	/// <summary> �G�̏�Ԃ��Z�b�g���� </summary>
	/// <param name="state"> �Z�b�g��������� </param>
	void SetState(EnemyState state);

	/// <summary> ��]�p�x��ݒ肷�� </summary>
	/// <param name="angle"> ��]�I���p�x </param>
	void SetRotationAngle(float angle);

	/// <summary> �K���ɉ�]������ </summary>
	/// <param name=""></param>
	void SetRotationAngle(void);

	/// <summary> ��]������ </summary>
	/// <param name="delta"></param>
	void Rotation(double delta);

	/// <summary> �ړ���������A�j���[�V������؂�ւ��� </summary>
	/// <param name=""></param>
	void CorrectMoveVecToAnim(void) final;

	/// <summary> ���݂̃X�e�[�g�ɂȂ��Ă���̌o�ߎ��Ԃ��擾���� </summary>
	/// <param name=""></param>
	/// <returns> �X�e�[�g�̌o�ߎ��� </returns>
	const float GetStateTime(void) const;

	/// <summary> �j�󎞂̏��� </summary>
	/// <param name=""></param>
	void Destroy(void) final;

	/// <summary> ����̕����Ɍo�H�T���ňړ����� </summary>
	/// <param name="pos"> �S�[�����W </param>
	/// <returns> ������true���s��false </returns>
	bool StartMoveNavi(const Math::Vector2& pos);

	bool StartMoveNavi(void);

	/// <summary> �ǐՂ��J�n���� </summary>
	/// <returns></returns>
	void TraceStart(void);

private:
	bool Init(std::vector<Math::Vector2I>& route);
	void Update(float delta) override; 
	void Draw(const Math::Vector2& offset, DrawMng& drawMng)override;

	ObjID GetObjID(void) override { return ObjID::Enemy; };      // id�̎擾

	/// <summary> �R���C�_�[�ɓn���悤�̖{�̂ɓ����������̏��� </summary>
	/// <param name="other"> ������������̃R���C�_�[ </param>
	void HitBody(Collider& other);

	/// <summary> �R���C�_�[�ɓn���p�̎��E�ɓ����������̏��� </summary>
	/// <param name="other"> ������������̏��� </param>
	void HitFun(Collider& other);

	/// <summary> �R���C�_�[�ɓn���p�̉��̔���ɓ����������̏��� </summary>
	/// <param name="other"> ������������̏��� </param>
	void HitSound(Collider& other);

	// �I�u�W�F�N�g���Ǘ����Ă���N���X
	ObjManager& objMng_;

	// �C�x���g���Ǘ����Ă���N���X
	std::weak_ptr< GmkEvent> gmkEvent_;

	// �G�̏��
	EnemyState state_;

	// �U��������x
	const float rotSpeed_ = Math::Deg2Rad(3.0f);

	// �U������p�x
	float targetAngle_ = 0.0f;

	// �U��������E�ǂ��炩
	float angleSing_ = 1.0f;

	// ���݂̃X�e�[�g�ɂȂ��Ă���̌o�ߎ���
	float stateTime_;

	// �ǐՂƂ��̃^�[�Q�b�g�̍��W
	Math::Vector2 targetPos_{-1.0f,-1.0f};

	// �v���C���[��ǂ��Ă��邩�̃t���O
	bool tracePlayerFlag_{ true };

	friend struct EnemyCheckAngleFunc;
	friend struct FanColIsHitFunc;

};

