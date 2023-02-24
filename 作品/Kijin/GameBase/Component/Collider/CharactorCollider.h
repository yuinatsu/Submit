#pragma once
#include "Collider.h"

// �G��v���C���[�̖{�̗p�����蔻��N���X(�J�v�Z���ɂ��낢��@�\������)
class CharactorCollider :
	public Collider
{
public:
	CharactorCollider();
	
	/// <summary>
	/// �㑤�̍��W
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetTop(void) const;
	
	/// <summary>
	/// �����̍��W
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3 GetBottom(void) const;

	/// <summary>
	/// ���a�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const float GetRadius(void) const
	{
		return radius_;
	}

	/// <summary>
	/// ���a�̃Z�b�g
	/// </summary>
	/// <param name="radius"></param>
	void SetRadius(const float radius)
	{
		radius_ = radius;
	}


	/// <summary>
	/// �ړ������̃Z�b�g
	/// </summary>
	/// <param name="moveDir"></param>
	void SetMoveDir(const Vector3& moveDir)
	{
		moveDir_ = moveDir;
	}

	/// <summary>
	/// �ړ������̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const Vector3& GetMoveDir(void) const&
	{
		return moveDir_;
	}

	/// <summary>
	/// �����̃Z�b�g
	/// </summary>
	/// <param name="height_"></param>
	void SetHeight(const float height)
	{
		height_ = height;
	}

	/// <summary>
	/// �����̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const float GetHeight(void) const
	{
		return  height_;
	}


	/// <summary>
	/// �X�s�[�h�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> �X�s�[�h </returns>
	const float GetSpeed(void) const
	{
		return speed_;
	}

	/// <summary>
	/// �X�s�[�h�̃Z�b�g
	/// </summary>
	/// <param name="speed"></param>
	void SetSpeed(const float speed)
	{
		speed_ = speed;
	}

	/// <summary>
	/// �n�ʂɐڒn���Ă��邩?
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const bool IsGround(void) const
	{
		return isGround_;
	}

	/// <summary>
	/// �ڒn�t���O���Z�b�g����
	/// </summary>
	/// <param name="flag"> �t���O </param>
	void SetGroundFlag(const bool flag)
	{
		isGround_ = flag;
	}

	/// <summary>
	/// �d�͂̒l���Z�b�g����
	/// </summary>
	/// <param name="g"></param>
	void SetGravityPow(const float g)
	{
		gravityPow_ = g;
	}

	/// <summary>
	/// �d�͂̒l���擾����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const float GetGravityPow(void) const
	{
		return std::abs(gravityPow_);
	}

#ifdef _DEBUG
	void DrawDebug(void) final;
#endif
	// �R���|�[�l���gID
	static constexpr ComponentID id_{ ComponentID::CapsuleCollider };
	ComponentID GetID(void) const override
	{
		return id_;
	}

private:
	void Check(Collider& col, ObjectManager& objectManager) final;
	bool Check(CapsuleCollider& col, ObjectManager& objectManager) final;
	bool Check(SphereCollider& col, ObjectManager& objectManager) final;
	bool Check(MeshCollider& col, ObjectManager& objectManager) final;
	bool Check(CharactorCollider& col, ObjectManager& objectManager) final;
	bool Check(TerrainCollider& col, ObjectManager& objectManager) final;

	// ����
	float height_;

	// ���a
	float radius_;

	// �ړ�����
	Vector3 moveDir_;

	// �ړ��X�s�[�h
	float speed_;

	// �ݒu���Ă��邩
	bool isGround_;

	// �d��(�������̒��n����p)
	float gravityPow_;
};

