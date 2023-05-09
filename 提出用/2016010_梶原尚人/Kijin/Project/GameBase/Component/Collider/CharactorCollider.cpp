#include "CharactorCollider.h"
#include "../../Object/ObjectManager.h"
#include "../../Common/Debug.h"

CharactorCollider::CharactorCollider() :
	height_{0.0f}, radius_{0.0f}, isGround_{false}, gravityPow_{1.0f}, speed_{0.0f}
{
	// �L�����N�^�[�͋����I�ɗL��
	isBlock_ = true;
}

const Vector3 CharactorCollider::GetTop(void) const
{
	if (!transform_.IsActive())
	{
		return offset_;
	}

	// �I�t�Z�b�g�����炵�����W�����߂�
	auto offsetVec = transform_->GetRotation() * offset_;
	Vector3 pos = transform_->Pos() + Vector3{offsetVec.x, offsetVec.y, offsetVec.z};

	// ����������炵�����W��Ԃ�
	pos += (transform_.Get()->GetUp() * height_);
	return pos;
}

const Vector3 CharactorCollider::GetBottom(void) const
{
	if (!transform_.IsActive())
	{
		return offset_;
	}
	auto offsetVec = transform_->GetRotation() * offset_;
	Vector3 pos = transform_->Pos() + Vector3{ offsetVec.x, offsetVec.y, offsetVec.z };

	// �����������炵�����W��Ԃ�
	pos += (-transform_.Get()->GetUp() * height_);
	return pos;
}

#ifdef _DEBUG

void CharactorCollider::DrawDebug(void)
{
	if (IsActive())
	{
		auto top{ GetTop() };
		auto btm{ GetBottom() };
		DebugDrawCapsule(top, btm, radius_, 0xffffff);
	}
}

#endif

void CharactorCollider::Check(Collider& col, ObjectManager& objectManager)
{
	if (col.IsActive() && IsActive())
	{
		if (col.Check(*this, objectManager))
		{
			// �����������̏���
			Hit(col, objectManager);
			col.Hit(*this, objectManager);
		}
	}
}

bool CharactorCollider::Check(CapsuleCollider& col, ObjectManager& objectManager)
{
	return Collider::HitCheck(col, *this);
}

bool CharactorCollider::Check(SphereCollider& col, ObjectManager& objectManager)
{
	return Collider::HitCheck(col, *this);
}

bool CharactorCollider::Check(MeshCollider& col, ObjectManager& objectManager)
{
	return Collider::HitCheck(col, *this);
}

bool CharactorCollider::Check(CharactorCollider& col, ObjectManager& objectManager)
{
	return Collider::HitCheck(*this, col, objectManager);
}

bool CharactorCollider::Check(TerrainCollider& col, ObjectManager& objectManager)
{
	return Collider::HitCheck(*this, col, objectManager);
}
