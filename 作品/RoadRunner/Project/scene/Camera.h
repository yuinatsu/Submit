#pragma once
#include <DxLib.h>
#include <memory>
#include "../common/Vector2.h"

class Obj;				// �v���g�^�C�v�錾

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
	std::weak_ptr<Obj> target_;				// �^�݂̂̋L���Ȃ̂Ńv���g�^�C�v�錾�݂̂Ŏ������
	Vector2 pos_;
	Vector2 defPos_;						// �f�t�H���g���W�p�ϐ�
	RECT antiMoveRect_;
};
