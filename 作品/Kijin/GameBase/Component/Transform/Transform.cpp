#include "../../Scene/BaseScene.h"
#include "../../Common/Math.h"
#include "Transform.h"
#include "../../Object/ObjectManager.h"
#include "../../Common/Debug.h"

Transform::Transform()
{
	rot_ = MGetIdent();
	localRot_ = MGetIdent();
	parentRot_ = MGetIdent();
}

void Transform::AddChild(const ObjectID& id)
{
	children_.emplace_back(id);
}

void Transform::RemoveChild(const ObjectID& id)
{
	children_.remove_if([&id](auto& c) {return *c == *id; });
}

void Transform::Update(BaseScene& scene, ObjectManager& objectManager, float delta, Controller& controller)
{
	DebugDrawLine(pos_, pos_ + (GetForward() * 50.0f), 0x0000ff);
	DebugDrawLine(pos_, pos_ + (GetUp() * 50.0f), 0x00ff00);
	DebugDrawLine(pos_, pos_ + (GetRight() * 50.0f), 0xff0000);

	// éqãüíBÇìØä˙Ç≥ÇπÇÈ
	for (auto& c : children_)
	{
		auto t = objectManager.GetComponent<Transform>(c);
		if (t.IsActive())
		{
			t->Sync(*this);
		}
	}

}

void Transform::Sync(Transform& parent)
{
	// êeÇ∆âÒì]ÇìØä˙Ç≥ÇπÇÈ
	parentRot_ = parent.GetRotationMatrix();
	//pos_ = parent.pos_;
}

void Transform::End(ObjectManager& objectManager)
{
	rot_ = MGetIdent();
	localRot_ = MGetIdent();
	parentRot_ = MGetIdent();
	offset_ = zeroVector3<float>;
	auto parent = objectManager.GetComponent<Transform>(parent_);
	if (parent.IsActive())
	{
		parent->RemoveChild(ownerId_);
	}

}

const Vector3 Transform::GetForward(void) const
{
	auto v{ VTransform(VGet(forwardVector3<float>.x, forwardVector3<float>.y, forwardVector3<float>.z), GetRotationMatrix()) };
	return {v.x,v.y,v.z};
}

const Vector3 Transform::GetUp(void) const
{
	auto v{ VTransform(VGet(upVector3<float>.x, upVector3<float>.y, upVector3<float>.z), GetRotationMatrix()) };
	return { v.x,v.y,v.z };
}

const Vector3 Transform::GetRight(void) const
{
	auto v{ VTransform(VGet(rightVector3<float>.x, rightVector3<float>.y, rightVector3<float>.z), GetRotationMatrix()) };
	return { v.x,v.y,v.z };
}

void Transform::LookAt(const Vector3& vec)
{
	auto fVec = VTransform(VGet(forwardVector3<float>.x, forwardVector3<float>.y, forwardVector3<float>.z), GetRotationMatrix());
	rot_ = MMult(rot_,MGetRotVec2(VGet(fVec.x, fVec.y, fVec.z), VGet(vec.x, vec.y, vec.z)));
}


void Transform::SetRotFromEulerRot(const Vector3& eulerRot)
{
	rot_ = MGetIdent();
	rot_ = MMult(rot_, MGetRotX(eulerRot.x));
	rot_ = MMult(rot_, MGetRotY(eulerRot.y));
	rot_ = MMult(rot_, MGetRotZ(eulerRot.z));
}

void Transform::AddRotFromEulerRot(const Vector3& eulerRot)
{
	rot_ = MMult(rot_, MGetRotX(eulerRot.x));
	rot_ = MMult(rot_, MGetRotY(eulerRot.y));
	rot_ = MMult(rot_, MGetRotZ(eulerRot.z));
}

void Transform::AddRotationMatrix(const MATRIX& matrix)
{
	rot_ = MMult(rot_, matrix);
}

void Transform::SetLocalRotFromEulerRot(const Vector3& eulerRot)
{
	localRot_ = MGetIdent();
	localRot_ = MMult(localRot_, MGetRotX(eulerRot.x));
	localRot_ = MMult(localRot_, MGetRotY(eulerRot.y));
	localRot_ = MMult(localRot_, MGetRotZ(eulerRot.z));
}

void Transform::AddLocalRotFromEulerRot(const Vector3& eulerRot)
{
	localRot_ = MMult(localRot_, MGetRotX(eulerRot.x));
	localRot_ = MMult(localRot_, MGetRotY(eulerRot.y));
	localRot_ = MMult(localRot_, MGetRotZ(eulerRot.z));
}

Transform& Transform::operator=(const Transform& transform)
{
	return *this;
}

void Transform::Read(std::ifstream& file)
{
	const auto& [pos, rot, scale] = Load(file);
	pos_ = pos;
	scale_ = scale;
	rot_ = MGetIdent();
	rot_ = MMult(rot_, MMult(MGetRotX(rot.x), MMult(MGetRotY(rot.y), MGetRotZ(rot.z))));
}

std::tuple<Vector3, Vector3, Vector3> Transform::Load(std::ifstream& file)
{
	Vector3 pos, rotation, scale;
	file.read(reinterpret_cast<char*>(&pos), sizeof(pos));
	file.read(reinterpret_cast<char*>(&rotation), sizeof(rotation));
	file.read(reinterpret_cast<char*>(&scale), sizeof(scale));
	return std::tie(pos, rotation, scale);
}

