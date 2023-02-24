#pragma once
#include "BaseScene.h"
#include "Obj/EditCur.h"

class EditScene :
	public BaseScene
{
public:
	EditScene();
	~EditScene();
private:
	uniqueBaseScene Update(uniqueBaseScene) override;
	SceneID GetSceneID() override;
	void DrawOwnScreen() override;
	void DrawOffsetScr();
	bool Init() override;

	Vector2 viewArea_;						// �P��ʂɕ\�������}�b�v�̍L��
	Vector2 worldArea_;						// �}�b�v�̍L��
	Vector2 chipSize_;						// �`�b�v�̃T�C�Y
	Vector2 drawOffset_;

	int gameScrID_;
};
