#pragma once
#include <memory>
#include "Common/Time.h"
#include "Common/Random.h"
#include "Common/Math.h"

#define lpApp (Application::GetInstance())
#define lpImageMng (Application::GetInstance().GetImageMng())
#define lpScreenMng (Application::GetInstance().GetScreenMng())

#define lpResourceMng (Application::GetInstance().GetResourceMng())
#define AppTime (Application::GetInstance().GetTimeClass())
#define Rand (Application::GetInstance().GetRandomClass())

// �A�v���P�[�V�����N���X�ɂ���X���b�h�v�[���N���X
#define AppThread (Application::GetInstance().GetThredPool())

class ResourceMng;
class BaseScene;
class ThreadPool;
class Controller;

class Application
{
public:

	/// <summary>
	/// ���s����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	int Run(void);

	static Application& GetInstance(void)
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
	/// �X���b�h�v�[����
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

	template<ValueC C>
	static constexpr Vector2Tmp<C> screenSize_{ 1280,720 };

private:
	Application();
	~Application();
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;
	bool Init(void);
	bool SysInit(void);
	static Application instance_;
	bool isInit_;
	bool isEnd_;
	std::unique_ptr<ResourceMng> resourceMng_;
	TimeClass timeClass_;
	Random random_;
	std::unique_ptr<BaseScene> scene_;
	std::unique_ptr<ThreadPool> threadPool_;
	std::unique_ptr<Controller> controller_;
	
};

