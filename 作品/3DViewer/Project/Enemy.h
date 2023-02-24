#pragma once
#include <dxlib.h>

class SceneManager;
class Unit;

// ����̍L��
static constexpr float VIEW_RANGE = 300.0f;
// ����p
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

	// ���f���̃n���h��ID
	int modelID_;

	// ���f���̕\�����W
	VECTOR pos_;

	// ���f���̊p�x
	VECTOR angles_;
	VECTOR anglesLocal_;

	// ���씻��
	bool isNotice_;
};

