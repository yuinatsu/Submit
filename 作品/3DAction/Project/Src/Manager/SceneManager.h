//#pragma once
#include <chrono>
#include <DxLib.h>
class SceneBase;
class ResourceManager;
class Fader;
class Camera;
class GravityManager;

// �V�[���Ǘ��p
enum class SCENE_ID
{
	NONE,
	TITLE,
	GAME
};

class SceneManager
{

public:

	void Init(void);
	void Init3D(void);
	void InitEffekseer(void);
	void Update(void);
	void Release(void);

	void ChangeScene(SCENE_ID nextId, bool isFading);

	// �f���^�^�C���̎擾
	float GetDeltaTime(void);

	// �J�����̎擾
	Camera* GetCamera(void);

	// �J�����̎擾
	ResourceManager* GetResourceManager(void);

	// �d�͕����̎擾
	GravityManager* GetGravityManager(void);

private:

	SCENE_ID mSceneID;
	SCENE_ID mWaitSceneID;

	SceneBase* mScene;
	ResourceManager* mResourceManager;
	Fader* mFader;
	Camera* mCamera;

	bool mIsSceneChanging;

	// �f���^�^�C��
	std::chrono::system_clock::time_point mTickCount;
	float mDeltaTime;

	// �d�͕����̐���
	GravityManager* mGravityManager;

	void ResetDeltaTime(void);

	void DoChangeScene(void);

};