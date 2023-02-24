#include <DxLib.h>
#include "Transform.h"

Transform::Transform(void)
{
	modelId = -1;
}

Transform::Transform(int model)
{
	modelId = model;
}

Transform::Transform(Transform* base)
{
	Copy(base);
}

void Transform::Update(void)
{

	// �傫��
	matScl = MGetScale(scl);

	// ��]
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();

	// �ʒu
	matPos = MGetTranslate(pos);

	// �s��̍���
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);

	// �s������f���ɔ���
	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}

	// �Փ˔���̍X�V
	if (collider != nullptr)
	{
		MV1RefreshCollInfo(modelId);
	}

}

void Transform::Release(void)
{

	if (collider != nullptr)
	{
		MV1TerminateCollInfo(modelId);
	}

}

void Transform::SetModel(int modelHId)
{
	modelId = modelHId;
}

void Transform::MakeCollider(Collider::TYPE type)
{

	if (modelId == -1)
	{
		return;
	}

	collider = new Collider(type, modelId);
	int ret = MV1SetupCollInfo(modelId, -1, 1, 1, 1);

}

VECTOR Transform::GetForward(void)
{
	return GetDir({ 0.0f, 0.0f, 1.0f });
}

VECTOR Transform::GetBack(void)
{
	return GetDir({ 0.0f, 0.0f, -1.0f });
}

VECTOR Transform::GetRight(void)
{
	return GetDir({ 1.0f, 0.0f, 0.0f });
}

VECTOR Transform::GetLeft(void)
{
	return GetDir({ -1.0f, 0.0f, 0.0f });
}

VECTOR Transform::GetUp(void)
{
	return GetDir({ 0.0f, 1.0f, 0.0f });
}

VECTOR Transform::GetDown(void)
{
	return GetDir({ 0.0f, -1.0f, 0.0f });
}

VECTOR Transform::GetDir(VECTOR vec)
{
	return quaRot.PosAxis(vec);
}

void Transform::Copy(Transform* base)
{
	if (base == nullptr)
	{
		return;
	}

	modelId = base->modelId;
	scl = base->scl;
	pos = base->pos;
	quaRot = base->quaRot;
	quaRotLocal = base->quaRotLocal;
	Update();
}
