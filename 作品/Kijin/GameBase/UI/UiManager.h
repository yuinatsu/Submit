#pragma once
#include <memory>
#include <list>
#include <vector>
#include <fstream>
#include "../Scene/SceneID.h"
#include "../Common/Vector2.h"

class BaseScene;
class ObjectManager;
class UiBase;
class Controller;

class UiManager
{
public:
	UiManager(SceneID scene);
	UiManager(SceneID scene, ResultAttribute atr);
	~UiManager();
	void InitUI(SceneID scene);
	void Begin();
	void Update(float delta, BaseScene& scene, ObjectManager& obj, Controller& controller);
	void Draw(void);

	const std::list<std::unique_ptr<UiBase>>& GetUiList(void) const { return uiList_; }
private:
	// �^�C�g���V�[���̍쐬
	void CreateTitleSceneUI(Vector2 pos, float interval);

	// �Z���N�g�V�[����UI�̍쐬
	void CreateSelectSceneUI(Vector2 pos, float interval);

	// �Q�[���V�[����UI�̍쐬
	void CreateGameSceneUI(Vector2 gaugePos, float interval, Vector2 comboPos);

	// ���U���g�V�[��UI�̍쐬
	void CreateResultSceneUi(Vector2 pos, float interval);

	// ���[�h�V�[��UI���쐬
	void CreateLoadSceneUI(Vector2 pos);

	// �|�[�Y�V�[��UI���쐬
	void CreatePauseSceneUI(Vector2 pos, float interval);

	/// <summary>
	/// �I�v�V�����V�[����UI
	/// </summary>
	/// <param name=""></param>
	void CreateOptionSceneUI(void);

	// UI�p�̃��X�g
	std::list<std::unique_ptr<UiBase>> uiList_;
	// �Ăяo�����V�[��
	SceneID scene_;
	// ���U���g����
	ResultAttribute result_;
};

