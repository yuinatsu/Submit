#pragma once
#include <memory>
#include <unordered_map>
#include "../Component/ComponentID.h"
#include "../common/Math.h"
#include "../input/Controller.h"
#include "../common/MapCollMng.h"
#include "../Component/Animation.h"
#include <string>
#include <deque>
#include "Effect/EffectID.h"
#include "Item/ItemName.h"

class Component;

using ComponentUPTr = std::unique_ptr<Component>;
using Components = std::deque<ComponentUPTr>;
using ComponentMap = std::unordered_map<ComponentID, Components>;

// �v���C���[�̏��
struct Move;
struct SpChange;
struct CheckKey;
struct SetAnime;
struct ColisionCheck;

// �`�揈���p�}�l�[�W���[
class DrawMng;

/// <summary> �I�u�W�F�N�g�̎�� </summary>
enum class ObjID
{
	Player,				// �v���C���[
	Enemy,				// �G
	UI,					// UI
	Box,				// �A�C�e���������Ă�Box
	Alert,				// �A���[�g
	TrapAlarm,			// �g���b�v�A���[��
	SnareFloor,			// �����Ȃ鏰
	LandMine,			// �n��
	Alcohol,			// �A���R�[��
	Decoy,				// �f�R�C
	Effect,				// �G�t�F�N�g
	Clock,				// ���v
	Target,				// �N���A�ڕW
	Decoration,			// �����p
	DoorL,				// �J����
	DoorS				// �J����
};

class Obj
{
public:
	Obj(const Math::Vector2& pos);
	virtual ~Obj();

	virtual void Update(float delta);
	virtual void Draw(const Math::Vector2& offset, DrawMng& drawMng);

	/// <summary> �R���|�[�l���g���擾���� </summary>
	/// <param name="id"> �擾�������R���|�[�l���g��ID </param>
	/// <returns> �R���|�[�l���g�̎Q�� </returns>
	const Components& GetAllComponent(ComponentID id) const;

	/// <summary> �R���|�[�l���g���擾���� </summary>
	/// <param name="id"> �擾�������R���|�[�l���g��ID </param>
	/// <returns> �R���|�[�l���g�̎Q�� </returns>
	Component& GetComponent(ComponentID id, int idx = 0) const;

	/// <summary> ID�ɊY������R���|�[�l���g���܂܂�邩 </summary>
	/// <param name="id"> �T�������R���|�[�l���g��ID </param>
	/// <returns></returns>
	bool IncludeComponent(ComponentID id) const;

	/// <summary> �R���|�[�l���g��ǉ����� </summary>
	/// <param name="component"> �ǉ��������R���|�[�l���g </param>
	/// <returns> ������true���s��false </returns>
	bool AddComponent(ComponentUPTr&& component);

	/// <summary> ���W���擾 </summary>
	/// <param name=""></param>
	/// <returns> ���W </returns>
	const Math::Vector2& GetPos(void) const;

	/// <summary> ���W���Z�b�g </summary>
	/// <param name="pos"> �Z�b�g���������W </param>
	void SetPos(const Math::Vector2& pos);

	/// <summary> ObjID�擾 </summary>
	/// <returns> ObjID </returns>
	virtual ObjID GetObjID(void) = 0;

	/// <summary> �ړ������x�N�g�����Z�b�g���� </summary>
	/// <param name="moveVec"></param>
	void SetMoveVec(const Math::Vector2& moveVec);

	/// <summary> �ړ������x�N�g�����擾���� </summary>
	/// <param name=""></param>
	/// <returns> �ړ������x�N�g�� </returns>
	const Math::Vector2& GetMoveVec(void) const;

	/// <summary> moveVec�Aspeed_�AspMag_�Adelta���g���ړ����� </summary>
	/// <param name="delta"></param>
	virtual void MovePos(float delta);

	/// <summary> �ړ��ł��邩�ǂ����H </summary>
	/// <returns> �ړ��ł���Ƃ�true�ł��Ȃ��Ƃ�false </returns>
	virtual const bool IsMove(float delta) const;

	/// <summary> �X�s�[�h���擾���� </summary>
	/// <param name=""> �X�s�[�h </param>
	/// <returns></returns>
	const float GetSpeed(void) const;

	/// <summary> �v���C���[�ړ��{���Z�b�g </summary>
	/// <param name="spMag"> �ړ��X�s�[�h�{�� </param>
	void SetSpMag(float spMag);

	/// <summary> �X�s�[�h�̔{�����擾���� </summary>
	/// <returns> �X�s�[�h�̔{�� </returns>
	const float GetSpMag(void) const;

	/// <summary> �������Ă��邩�H </summary>
	/// <returns> �������Ă���Ƃ�true���Ă��Ȃ��Ƃ�false </returns>
	const bool IsAlive(void) const { return alive_; }

	/// <summary> ���̃I�u�W�F�N�g��j�����邽�߂ɐ����t���O��false�ɂ���(�I�u�W�F�N�g�ŗL�̏��������Ă���ɂ������ꍇ�̓I�[�o�[���C�h���Ă�) </summary>
	virtual void Destroy(void) { alive_ = false; }

	/// <summary> ����������G�t�F�N�g���擾 </summary>
	/// <returns> EffectName </returns>
	EffectName GetEffect(void) const { return effect_; }

	/// <summary> �U�����t���O </summary>
	/// <returns> �U����:true�A��:false </returns>
	const bool IsAttack(void) const { return isAtk_; }

	/// <summary> �U���t���O�Z�b�g </summary>
	/// <param name="flag"> �U�����t���O </param>
	void SetAttackFlag(bool flag) { isAtk_ = flag; }
protected:

	/// <summary> �R���|�[�l���g�̍X�V���� </summary>
	/// <param name="delta"></param>
	void UpdateComponent(float delta) const;

	/// <summary> ���W </summary>
	Math::Vector2 pos_;

	/// <summary> �ړ����� </summary>
	Math::Vector2 moveVec_;

	/// <summary> �ړ��� </summary>
	float speed_;

	/// <summary> �ړ��ʔ{�� </summary>
	float spMag_;

	/// <summary> �f���^�^�C�� </summary>
	float delta_;

	/// <summary> �����t���O </summary>
	bool alive_;

	/// <summary> �U�����t���O </summary>
	bool isAtk_;

	/// <summary> ����������G�t�F�N�g </summary>
	EffectName effect_;

	ItemName item_;

	// �t�����h
	friend Move;
	friend SpChange;
	friend CheckKey;
	friend SetAnime;
	friend ColisionCheck;
	friend struct MovePos;
	friend struct Attack;
private:
	/// <summary> �R���|�[�l���g�ނ��}�b�v�Ŏ��� </summary>
	ComponentMap componentList_;
};

