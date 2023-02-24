#pragma once
#include "BaseScene.h"
#include "Input/Input.h"

class ClearScene :
	public BaseScene
{
public:
	ClearScene();
	~ClearScene();
private:
	uniqueBaseScene Update(uniqueBaseScene own) override;
	SceneID GetSceneID() override;
	void DrawOwnScreen() override;
	bool Init() override;

	std::unique_ptr <Input> input_;		// “ü—ÍŒnî•ñ
	int clearScrID_;
};

