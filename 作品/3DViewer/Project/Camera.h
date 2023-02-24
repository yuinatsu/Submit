#pragma once
#include <dxlib.h>
#include "Unit.h"

// �J�����̍���
static constexpr float HEIGHT = 200.0f;

// �����_����J�����܂ł�XZ����
static constexpr float DIS_TARGET_TO_CAMERA = 500.0f;

// �����_����L�����܂ł�XZ����
static constexpr float DIS_TARGET_TO_UNIT = 300.0f;

class Camera
{
public:
	Camera();
	~Camera();
	void Init(void);
	void Update(void);
	void SetBeforDraw(void);
	void Draw(void);
	void Release(void);

	void SetUnit(Unit* unit);

	VECTOR GetPos(void);
	VECTOR GetAngles(void);
	VECTOR GetTargetPos(void);

private:

	Unit* unit_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����p�x(rad)
	VECTOR angles_;

	// �����_
	VECTOR targetPos_;
};

