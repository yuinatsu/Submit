#pragma once
#include <chrono>
#include "TitleScene.h"
#include "GameScene.h"
class SceneBase;
class Fader;
class Camera;
class MiniCamera;
class ResourceManager;
class ParticleGenerator;

class SceneManager
{

public:

	// �V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		EVENT,
		BATTLE
	};

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Release(void);

	void ChangeScene(SCENE_ID nextId, bool isFading);

	SCENE_ID GetmSceneID(void);

	// �f���^�^�C���̎擾
	float GetDeltaTime(void);

	Camera* GetCamera(void);

	ResourceManager* GetResource(void);

private:

	SCENE_ID mSceneID;
	SCENE_ID mWaitSceneID;

	SceneBase* mScene;
	Fader* mFader;
	Camera* mCamera;
	MiniCamera* mMiniCamera;

	ResourceManager* mResourceManager;
	ParticleGenerator* particleGenerator_;

	bool mIsSceneChanging;

	// �f���^�^�C��
	std::chrono::system_clock::time_point mTickCount;
	double mDeltaTime;

	void ResetDeltaTime(void);

	void DoChangeScene(void);

};