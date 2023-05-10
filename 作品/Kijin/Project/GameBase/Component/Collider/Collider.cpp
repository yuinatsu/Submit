#include "Collider.h"
#include "CapsuleCollider.h"
#include "SphereCollider.h"
#include "MeshCollider.h"
#include "CharactorCollider.h"
#include "TerrainCollider.h"
#include "../../Common/Debug.h"
#include "../Behavior/PlayerBehavior.h"

Collider::~Collider()
{
}

void Collider::Begin(ObjectManager& objectManager)
{
	transform_ = objectManager.GetComponent<Transform>(ownerId_);
	isActive_ = true;
}

void Collider::End(ObjectManager& objectManager)
{
	offset_ = zeroVector3<float>;
	hit_ = nullptr;
}

void Collider::Hit(Collider& col, ObjectManager& objMng)
{
	if (hit_)
	{
		// �q�b�g���ɌĂяo���ׂ�function�����鎞�Ăяo��
		hit_(col, objMng);
	}
}

bool Collider::HitCheck(CapsuleCollider& a, CapsuleCollider& b)
{
	auto aTop{ a.GetTop() };
	auto aBotm{ a.GetBottom() };

	auto bTop{ b.GetTop() };
	auto bBotm{ b.GetBottom() };

	return HitCheck_Capsule_Capsule(
		VGet(bTop.x, bTop.y, bTop.z), VGet(bBotm.x, bBotm.y, bBotm.z), b.GetRadius(),
		VGet(aTop.x, aTop.y, aTop.z), VGet(aBotm.x, aBotm.y, aBotm.z), a.GetRadius()) == TRUE;
}

bool Collider::HitCheck(CapsuleCollider& a, SphereCollider& b)
{
	auto r{ b.GetRadius() };
	auto pos{ b.GetPos() };


	auto aTop{ a.GetTop() };
	auto aBotm{ a.GetBottom() };

	return HitCheck_Sphere_Capsule(
		VGet(pos.x, pos.y, pos.z), r,
		VGet(aTop.x, aTop.y, aTop.z), VGet(aBotm.x, aBotm.y, aBotm.z), b.GetRadius()) == TRUE;
}

bool Collider::HitCheck(CapsuleCollider& a, MeshCollider& b)
{
	auto aTop{ a.GetTop() };
	auto aBotm{ a.GetBottom() };

	auto result = MV1CollCheck_Capsule(b.GetHandle(), -1, VGet(aTop.x, aTop.y, aTop.z), VGet(aBotm.x, aBotm.y, aBotm.z), a.GetRadius());
	auto ret{ result.HitNum > 0 };
	DxLib::MV1CollResultPolyDimTerminate(result);
	return ret;
}

bool Collider::HitCheck(CapsuleCollider& a, CharactorCollider& b)
{
	// �J�v�Z���̃g�b�v�ƃ{�g���̍��W���擾
	auto aTop{ a.GetTop() };
	auto aBotm{ a.GetBottom() };

	// �L�����N�^�[�̃g�b�v�ƃ{�g���̍��W���擾
	auto bTop{ b.GetTop() };
	auto bBotm{ b.GetBottom() };

	return HitCheck_Capsule_Capsule(
		VGet(bTop.x, bTop.y, bTop.z), VGet(bBotm.x, bBotm.y, bBotm.z), b.GetRadius(),
		VGet(aTop.x, aTop.y, aTop.z), VGet(aBotm.x, aBotm.y, aBotm.z), a.GetRadius()) == TRUE;

}

bool Collider::HitCheck(CapsuleCollider& a, TerrainCollider& b)
{
	auto aTop{ a.GetTop() };
	auto aBotm{ a.GetBottom() };

	auto result = MV1CollCheck_Capsule(b.GetHandle(), -1, VGet(aTop.x, aTop.y, aTop.z), VGet(aBotm.x, aBotm.y, aBotm.z), a.GetRadius());
	auto ret{ result.HitNum > 0 };
	DxLib::MV1CollResultPolyDimTerminate(result);
	return ret;
}

bool Collider::HitCheck(SphereCollider& a, SphereCollider& b)
{
	auto r{ Square(a.GetRadius() + b.GetRadius())};
	auto sqLength{ (a.GetPos() - b.GetPos()).SqMagnitude() };
	return (r >= sqLength);
}

bool Collider::HitCheck(SphereCollider& a, MeshCollider& b)
{
	auto r{ a.GetRadius() };
	auto pos{ a.GetPos() };
	auto result = MV1CollCheck_Sphere(b.GetHandle(), -1, VGet(pos.x, pos.y, pos.z), r);
	auto ret{ result.HitNum > 0 };
	DxLib::MV1CollResultPolyDimTerminate(result);
	return ret;
}

bool Collider::HitCheck(SphereCollider& a, CharactorCollider& b)
{
	// ���̂̏����擾
	auto r{ a.GetRadius() };
	auto pos{ a.GetPos() };

	// �J�v�Z���̃g�b�v�ƃ{�g���̍��W���擾
	auto bTop{ b.GetTop() };
	auto bBotm{ b.GetBottom() };

	return HitCheck_Sphere_Capsule(
		VGet(pos.x, pos.y, pos.z), r,
		VGet(bTop.x, bTop.y, bTop.z), VGet(bBotm.x, bBotm.y, bBotm.z), b.GetRadius()) == TRUE;
}

bool Collider::HitCheck(SphereCollider& a, TerrainCollider& b, ObjectManager& objectmanager)
{
	// ���̂̏����擾
	auto r{ a.GetRadius() };
	auto pos{ a.GetPos() };

	auto result = MV1CollCheck_Sphere(b.GetHandle(), -1, VGet(pos.x, pos.y, pos.z), r);
	auto ret{ result.HitNum > 0 };

	if (ret && a.IsBlock())
	{
		// �������Ă��Ă������o������������Ƃ�
		auto trans = objectmanager.GetComponent<Transform>(a.GetOwnerID());
		for (int i = 0; i < result.HitNum; i++)
		{
			for (int tcount = 0; tcount < 10; tcount++)
			{
				if (!HitCheck_Sphere_Triangle(VGet(pos.x, pos.y, pos.z), r, result.Dim[i].Position[0], result.Dim[i].Position[1], result.Dim[i].Position[2]))
				{
					break;
				}
				trans->Pos() += Vector3{ result.Dim[i].Normal.x,result.Dim[i].Normal.y, result.Dim[i].Normal.z}.Normalized() * 1.0f;
				pos = a.GetPos();
				r = a.GetRadius();
				
			}
		}
	}

	DxLib::MV1CollResultPolyDimTerminate(result);
	return ret;
}

bool Collider::HitCheck(MeshCollider& a, MeshCollider& b)
{
	// �����蔻�肵�Ȃ�
	return false;
}

bool Collider::HitCheck(MeshCollider& a, CharactorCollider& b)
{
	auto bTop{ b.GetTop() };
	auto bBotm{ b.GetBottom() };

	auto result = MV1CollCheck_Capsule(a.GetHandle(), -1, VGet(bTop.x, bTop.y, bTop.z), VGet(bBotm.x, bBotm.y, bBotm.z), b.GetRadius());
	auto ret{ result.HitNum > 0 };
	DxLib::MV1CollResultPolyDimTerminate(result);
	return ret;
}

bool Collider::HitCheck(MeshCollider& a, TerrainCollider& b)
{
	// �����蔻�肵�Ȃ�
	return false;
}

bool Collider::HitCheck(CharactorCollider& a, CharactorCollider& b, ObjectManager& objectManager)
{
	// ���ꂼ��̃g�b�v�ƃ{�g���̍��W���擾
	auto atop{ a.GetTop() };
	auto abotm{ a.GetBottom() };
	auto bBtm{ b.GetBottom() };
	auto bTop{ b.GetTop() };

	// �ŒZ���������߂�
	auto dis = Segment_Segment_MinLength(
		VGet(atop.x, atop.y, atop.z), VGet(abotm.x, abotm.y, abotm.z),
		VGet(bBtm.x, bBtm.y, bBtm.z), VGet(bTop.x, bTop.y, bTop.z));

	if (dis <= a.GetRadius() + b.GetRadius())
	{
		// ���������������o������������
		auto colT = objectManager.GetComponent<Transform>(a.GetOwnerID());
		auto myT = objectManager.GetComponent<Transform>(b.GetOwnerID());

		// �����o���悤�x�N�g�������
		auto vec{ colT->GetPos() - myT->GetPos()};

		// �㉺�ɂ͉����o���Ȃ��̂�
		vec.y = 0.0f;
		vec.Normalize();

		auto deff = (a.GetRadius() + b.GetRadius() - dis);
		colT->Pos() += vec * (deff * 0.5f);
		myT->Pos() += -vec * (deff * 0.5f);
		return true;
	}
	return false;
}

bool Collider::HitCheck(CharactorCollider& a, TerrainCollider& b, ObjectManager& objectManager)
{
	// �J�v�Z���̃g�b�v�ƃ{�g���̍��W���擾
	auto aTop{ a.GetTop() };
	auto aBtm{ a.GetBottom() };

	// �g�����X�t�H�[�����擾
	auto trans = objectManager.GetComponent<Transform>(a.GetOwnerID());

	// �ڒn����
	// ����p�̏I�_�����߂�
	auto aEnd{ aBtm + (downVector3<float> *(a.GetRadius())) };
	auto aStart{ aEnd + (upVector3<float> * a.GetGravityPow()) };

	// �������߂ɔ�������悤�ɂ���
	constexpr auto overSize{ 10.0f };
	aEnd += downVector3<float> * overSize;
	aStart += upVector3<float> *overSize;

	// �`�F�b�N
	auto lineResult = MV1CollCheck_Line(b.GetHandle(), -1, VGet(aStart.x, aStart.y, aStart.z), VGet(aEnd.x, aEnd.y, aEnd.z));
	if (lineResult.HitFlag > 0)
	{
		// �����������W
		auto hitPos = Vector3{ lineResult.HitPosition.x, lineResult.HitPosition.y , lineResult.HitPosition.z };

		// ���������|���S���̖@��
		auto hitNormal = Vector3{ lineResult.Normal.x , lineResult.Normal.y, lineResult.Normal.z };

		// �d��
		auto g = downVector3<float> *(a.GetGravityPow() > 0 ? a.GetGravityPow() : 4.9f);

		// ���K��
		hitNormal.Normalize();

		// ���������|���S���̖@�������ƂɌX�΂��l�������x�N�g���𐶐�(v = g - n(n * g))
		auto v = g - (hitNormal * Dot(hitNormal, g));

		trans->Pos() += v ;
		a.SetGroundFlag(true);
		a.SetGravityPow(0.0f);

		// �␳���|�����̂ō��W���Ď擾
		aTop = a.GetTop();
		aBtm = a.GetBottom();
	}
	else
	{
		a.SetGroundFlag(false);
	}
	

	


	// �_�b�V�����l��������������
	auto topDashStart = aTop + (-a.GetMoveDir() * a.GetSpeed());
	auto btmDashStart = aBtm + (-a.GetMoveDir() * a.GetSpeed());
	
	// �㉺��_���瓖���蔻������
	auto topResult = MV1CollCheck_Line(b.GetHandle(), -1, VGet(topDashStart.x, topDashStart.y, topDashStart.z),VGet(aTop.x, aTop.y, aTop.z));
	auto btmResult = MV1CollCheck_Line(b.GetHandle(), -1, VGet(btmDashStart.x, btmDashStart.y, btmDashStart.z), VGet(aBtm.x, aBtm.y, aBtm.z));
	bool ret = false;
	if (topResult.HitFlag > 0 || btmResult.HitFlag > 0)
	{
		ret = true;
		if (topResult.HitFlag > 0 && btmResult.HitFlag > 0)
		{
			// �㉺�����������Ă鎞
			auto topHitPos{ Vector3{topResult.HitPosition.x,topResult.HitPosition.y, topResult.HitPosition.z} };
			auto btmHitPos{ Vector3{btmResult.HitPosition.x,btmResult.HitPosition.y, btmResult.HitPosition.z} };
			DebugDrawSphere(topHitPos, 50.0f, 0xff0000);
			DebugDrawSphere(btmHitPos, 50.0f, 0xff0000);
			if ((topHitPos - topDashStart).SqMagnitude() < (btmHitPos - btmDashStart).SqMagnitude())
			{
				// ��̕������̈ʒu����̌�_���߂��Ƃ�
				trans->Pos() += -a.GetMoveDir() * ((a.GetSpeed() - (topHitPos - topDashStart).Magnitude()) + a.GetRadius());
			}
			else
			{
				// ���̕������̈ʒu����̌�_���߂��Ƃ�
				trans->Pos() += -a.GetMoveDir() * ((a.GetSpeed() - (btmHitPos - btmDashStart).Magnitude()) + a.GetRadius());
			}
		}
		else
		{
			if (topResult.HitFlag > 0)
			{
				// ��̕������̈ʒu����̌�_���߂��Ƃ�
				auto topHitPos{ Vector3{topResult.HitPosition.x,topResult.HitPosition.y, topResult.HitPosition.z} };
				trans->Pos() += -a.GetMoveDir() * ((a.GetSpeed() - (topHitPos - topDashStart).Magnitude()) + a.GetRadius());
			}
			else
			{
				// ���̕������̈ʒu����̌�_���߂��Ƃ�
				auto btmHitPos{ Vector3{btmResult.HitPosition.x,btmResult.HitPosition.y, btmResult.HitPosition.z} };
				trans->Pos() += -a.GetMoveDir() * ((a.GetSpeed() - (btmHitPos - btmDashStart).Magnitude()) + a.GetRadius());
			}
		}

		// ���W�ɕ␳���|�����̂ōĎ擾
		aTop = a.GetTop();
		aBtm = a.GetBottom();
	}
	

	// �n�`�Ƃ̔���
	auto result{ MV1CollCheck_Capsule(b.GetHandle(), -1 , VGet(aTop.x, aTop.y, aTop.z), VGet(aBtm.x, aBtm.y, aBtm.z), a.GetRadius()) };
	if (result.HitNum > 0)
	{
		ret = true;
		for (int i = 0; i < result.HitNum; i++)
		{
			// �����o������
			for (int tcount = 0; tcount < 50; tcount++)
			{
				if (!HitCheck_Capsule_Triangle(
					VGet(aTop.x, aTop.y, aTop.z), VGet(aBtm.x, aBtm.y, aBtm.z), a.GetRadius(),
					result.Dim[i].Position[0], result.Dim[i].Position[1], result.Dim[i].Position[2]
				))
				{
					// �|���S���Ɠ������ĂȂ��Ƃ��̃��[�v�𔲂���
					break;
				}

				// �|���S���̖@�������ɉ����o��
				trans->Pos() += Vector3{ result.Dim[i].Normal.x, result.Dim[i].Normal.y, result.Dim[i].Normal.z }.Normalized() * 1.0f;
				aTop = a.GetTop();
				aBtm = a.GetBottom();
			}
		}
	}

	// �����蔻�����j������
	MV1CollResultPolyDimTerminate(result);

	return ret;
}

bool Collider::HitCheck(TerrainCollider& a, TerrainCollider& b)
{
	// �����蔻������Ȃ�
	return false;
}
