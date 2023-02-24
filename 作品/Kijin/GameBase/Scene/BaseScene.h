#pragma once
#include <memory>
#include <functional>
#include <unordered_map>
#include <list>
#include "SceneID.h"
#include "../Component/ComponentID.h"
#include "../Common/SharedHandle.h"
#include "../Common/ScreenID.h"
#include "../Common/Input/Controller.h"

class BaseScene;
class Camera;
class ObjectManager;
class Controller;
class UiManager;

class BaseScene
{
public:
	using SceneUptr = std::unique_ptr<BaseScene>;
	using LoadedFunc = std::function<void(Controller&)>;

	/// <summary> 
	/// �R���X�g���N�^ 
	/// </summary>
	/// <param name="id"> �V�[�� </param>
	/// <param name="sceneID"> �����V�[�� </param>
	/// <param name="maxObj"> �I�u�W�F�N�g�̍ő吔 </param>
	BaseScene(ScreenID id, SceneID sceneID);

	/// <summary>
	/// �f�X�g���N�^ 
	/// </summary>
	virtual ~BaseScene();

	/// <summary>
	///  �X�V
	/// </summary>
	/// <param name="delta"> �f���^�^�C�� </param>
	virtual void Update(float delta, Controller& controller);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(void);

	/// <summary>
	/// �V�[���̓��e�̕`��
	/// </summary>
	virtual void DrawScene(void) = 0;

	/// <summary>
	/// �V�[����ID�̎擾
	/// </summary>
	/// <returns> �V�[����ID </returns>
	virtual const SceneID GetID(void) const = 0;

	/// <summary>+
	/// �V�[�����̉������
	/// </summary>
	virtual void Relese(void);

	/// <summary>
	/// ���[�h�������������H
	/// </summary>
	/// <returns> true�̎����[�h���� </returns>
	virtual bool IsLoaded(void);

	/// <summary>
	/// �V�[���̃|�C���^�̍X�V
	/// </summary>
	/// <param name="own"> ���݂̃V�[���̃|�C���^ </param>
	/// <returns> ���̃t���[���Ŏg���|�C���^ </returns>
	SceneUptr Update(SceneUptr own, float delta, Controller& controller);
	
	/// <summary>
	/// �V�[����ς���
	/// </summary>
	/// <param name="id"> �ύX��̃V�[����ID </param>
	void ChangeSceneID(SceneID id);

	/// <summary>
	/// �o�b�N�V�[���ɕύX����
	/// </summary>
	/// <param name=""></param>
	void ChangeBackScene(void);

	/// <summary>
	/// �V�[���𐶐����邽�߂�function���Z�b�g����
	/// </summary>
	/// <param name="sceneMakeFunc"> �������邽�߂�function </param>
	/// <param name="id"> ��������V�[����ID </param>
	void SetMakeSceneFunc(std::function<SceneUptr(SceneUptr)>&& sceneMakeFunc, const SceneID id);

	/// <summary>
	/// ���[�h�������̏���
	/// </summary>
	void Loaded(Controller& controller);

	/// <summary>
	/// ���[�h�������̏�����ǉ�����
	/// </summary>
	/// <param name="loadedFunc"></param>
	void AddLoadedFunc(LoadedFunc&& loadedFunc);

	/// <summary>
	/// ���̃V�[�����Z�b�g
	/// </summary>
	/// <param name="back"></param>
	void SetBackScene(SceneUptr back);

	/// <summary>
	/// �O�̃V�[�����Z�b�g
	/// </summary>
	/// <param name="front"></param>
	void SetFrontScene(SceneUptr front);
	
	/// <summary>
	/// �J�����̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns> �J�����̎Q�� </returns>
	Camera& GetCamera(void)
	{
		return *camera_;
	}

	/// <summary>
	/// �I�u�W�F�N�g�}�l�[�W���[�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	ObjectManager& GetObjectManager(void)
	{
		return *objMng_;
	}

	/// <summary>
	/// �o�b�N�V�[������̐؂�ւ����Ɏ��s�����
	/// </summary>
	/// <param name=""></param>
	virtual void ChangeBackFront(void);

protected:

	/// <summary>
	/// �o�b�N�V�[���̃X�N���[���̓��e��`�悷��
	/// </summary>
	/// <param name=""></param>
	void DrawSceneBackScene(void);

	/// <summary>
	/// �o�b�N�V�[����`�悷��
	/// </summary>
	/// <param name=""></param>
	void DrawBackScene(void);
	
	// ���̃V�[��(���j���[�Ƃ��|�[�Y���Ŏg��)
	SceneUptr back_;

	// �O�ɂ���V�[��(���j���[�Ƃ��|�[�Y�Ƃ������Ďg��)
	std::unordered_map<SceneID,SceneUptr> front_;

	// ���ڍs����V�[��ID
	SceneID nextID_;

	// �X�N���[��ID
	ScreenID scID_;

	// �X�N���[���n���h��
	SharedRenderTargetHandle screenHandle_;

	// �J����
	std::unique_ptr<Camera> camera_;

	// �I�u�W�F�N�g�}�l�[�W���[
	std::unique_ptr<ObjectManager> objMng_;

	// UI�}�l�[�W���[
	std::unique_ptr<UiManager> uiMng_;


	// back�V�[����`�悷�邩
	bool isDrawBackScene_;

	// back�V�[�����X�V���邩
	bool isUpdateBackScene_;
private:

	// �V�[���쐬��function���܂Ƃ߂��}�b�v
	std::unordered_map<SceneID, std::function<SceneUptr(SceneUptr)>> sceneMakeFuncMap_;

	// ���[�h�������Ɏ��s����function
	std::list<LoadedFunc> loadedFunc_;
};

