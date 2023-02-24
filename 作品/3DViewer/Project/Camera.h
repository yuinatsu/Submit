#pragma once
#include <dxlib.h>
#include "Unit.h"

// カメラの高さ
static constexpr float HEIGHT = 200.0f;

// 注視点からカメラまでのXZ距離
static constexpr float DIS_TARGET_TO_CAMERA = 500.0f;

// 注視点からキャラまでのXZ距離
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

	// カメラの位置
	VECTOR pos_;

	// カメラ角度(rad)
	VECTOR angles_;

	// 注視点
	VECTOR targetPos_;
};

