#pragma once
#include <dxlib.h>
#include <vector>
#include <memory>
class SceneManager;
class Bullet;

// アニメーション速度
static constexpr float SPEED_ANIM = 20.0f;

// 徐々に回転する量
static constexpr float SPEED_ROT = 5.0f;
static constexpr float SPEED_ROT_RAD = SPEED_ROT * (DX_PI_F / 180.0f);

class Unit
{
public:
	Unit(SceneManager* manager);
	void Init();
	void Update();
	void Draw();
	void Release();

	VECTOR GetPos(void);
	VECTOR GetAngles(void);

	char keyBuf[256];				// 現在のキーバッファ
	char keyBufOld[256];			// １つ前のキーバッファ
private:
	SceneManager* sceneMng_;
	std::vector<std::shared_ptr<Bullet>> bulet_;

	// モデルのハンドルID
	int modelID_;

	// アニメーション用のハンドルID
	int animWalkID_;

	// モデルの表示座標
	VECTOR pos_;

	// モデルの角度
	VECTOR angles_;
	VECTOR anglesLocal_;

	// アニメーションの総再生時間
	float timeTotalAnimWalk_;

	// アニメーション用の更新ステップ
	float stepAnim_;
};

