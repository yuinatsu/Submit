#pragma once
#include <dxlib.h>

class SceneManager;
class Unit;

// 視野の広さ
static constexpr float VIEW_RANGE = 300.0f;
// 視野角
static constexpr float VIEW_ANGLE = 30.0f;

class Enemy
{
public:
	Enemy(SceneManager* manager, Unit* unit);
	void Init();
	void Update();
	void Draw();
	void Release();

	VECTOR GetPos(void);
	VECTOR GetAngles(void);
private:
	SceneManager* sceneMng_;
	Unit* unit_;

	// モデルのハンドルID
	int modelID_;

	// モデルの表示座標
	VECTOR pos_;

	// モデルの角度
	VECTOR angles_;
	VECTOR anglesLocal_;

	// 視野判定
	bool isNotice_;
};

