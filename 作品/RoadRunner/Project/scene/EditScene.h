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

	Vector2 viewArea_;						// １画面に表示されるマップの広さ
	Vector2 worldArea_;						// マップの広さ
	Vector2 chipSize_;						// チップのサイズ
	Vector2 drawOffset_;

	int gameScrID_;
};
