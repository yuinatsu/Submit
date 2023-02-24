#pragma once
#include <DxLib.h>
#include <memory>
#include "../common/Vector2.h"

class Obj;				// プロトタイプ宣言

class Camera
{
public:
	Camera();
	~Camera();
	bool InitArea(void);						// 
	bool SetTarget(std::weak_ptr<Obj> target);
	void Update(void);
	void Draw(void);
	void SetPos(const Vector2& pos);
	const Vector2& GetPos(void);
	const Vector2 GetDrawOffset(void);
private:
	std::weak_ptr<Obj> target_;				// 型のみの記入なのでプロトタイプ宣言のみで事足りる
	Vector2 pos_;
	Vector2 defPos_;						// デフォルト座標用変数
	RECT antiMoveRect_;
};
