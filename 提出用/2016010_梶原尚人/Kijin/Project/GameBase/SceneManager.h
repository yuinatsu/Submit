#pragma once
#include <memory>
#include "Common/Time.h"
#include "Common/Random.h"
#include "Common/Math.h"

#define lpSceneMng (SceneManager::GetInstance())
#define Rand (SceneManager::GetInstance().GetRandomClass())
#define Thread (SceneManager::GetInstance().GetThredPool())

class ResourceMng;
class BaseScene;
class ThreadPool;
class Controller;

class SceneManager
{
public:

	/// <summary>
	/// ���s����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int Run(void);

	void Draw();

	void Update();

	static SceneManager& GetInstance(void)
	{
		return instance_;
	}

	/// <summary>
	/// ���\�[�X�̊Ǘ��N���X�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	ResourceMng& GetResourceMng(void);

	/// <summary>
	/// �^�C���N���X�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	TimeClass& GetTimeClass(void);

	/// <summary>
	/// �����N���X�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	Random& GetRandomClass(void);

	/// <summary>
	/// �X���b�h�v�[��
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	ThreadPool& GetThredPool(void);

	/// <summary>
	/// �v���O�������I������
	/// </summary>
	/// <param name=""></param>
	void End(void)
	{
		isEnd_ = true;
	}

	// �X�N���[���T�C�Y
	template<ValueC C>
	static constexpr Vector2Tmp<C> screenSize_{ 1280,720 };

	/// <summary>
	/// �R���g���[���[�̎擾
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	Controller& GetController(void);
private:
	SceneManager();
	~SceneManager();
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name=""></param>
	/// <returns> �������Atrue���s���Afalse </returns>
	bool Init(void);

	/// <summary>
	/// �V�X�e���n�̏�����
	/// </summary>
	/// <param name=""></param>
	/// <returns> �������Atrue���s���Afalse </returns>
	bool SysInit(void);

	// ���̃N���X�̃C���X�^���X
	static SceneManager instance_;

	// �������ł������̃t���O
	bool isInit_;

	// �I�����邩�̃t���O
	bool isEnd_;

	// ���\�[�X�}�l�[�W���[
	std::unique_ptr<ResourceMng> resourceMng_;

	// �^�C���N���X
	TimeClass timeClass_;

	// �����_���N���X
	Random random_;

	// �V�[��
	std::unique_ptr<BaseScene> scene_;

	// �X���b�h�v�[��
	std::unique_ptr<ThreadPool> threadPool_;

	// �R���g���[���[
	std::unique_ptr<Controller> controller_;
	
};

