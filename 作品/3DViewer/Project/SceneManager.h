#pragma once
#include <chrono>
#include "TitleScene.h"
class SceneBase;
class Fader;
class Camera;
class MiniCamera;

// �V�[���Ǘ��p
enum class SCENE_ID
{
	NONE, 
	TITLE
};

class SceneManager
{

public:

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Release(void);

	void ChangeScene(SCENE_ID nextId, bool isFading);

	// �f���^�^�C���̎擾
	float GetDeltaTime(void);

	// �J�����̎擾
	Camera* GetCamera(void);

private:

	SCENE_ID sceneID_;
	SCENE_ID waitSceneID_;

	SceneBase* scene_;
	Fader* fader_;
	Camera* camera_;
	MiniCamera* miniCamera_;

	bool isSceneChanging_;

	// �f���^�^�C��
	std::chrono::system_clock::time_point tickCount_;
	float deltaTime_;

	void DoChangeScene(void);

};