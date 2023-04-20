#pragma once
#include <thread>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <mutex>

class ThreadPool;

// ��Ɨp�X���b�h�����N���X
class Worker
{
public:
	Worker(ThreadPool& threadPool);
	~Worker();

	/// <summary>
	/// �^�X�N(���s����function)���Z�b�g����
	/// </summary>
	/// <param name="task"> �^�X�N </param>
	void SetTask(std::function<void(void)>&& task);

	/// <summary>
	/// �������~����
	/// </summary>
	/// <param name=""></param>
	void Stop(void);

	/// <summary>
	/// Join����
	/// </summary>
	/// <param name=""></param>
	void Join(void);

	/// <summary>
	/// �^�X�N���I�����Ă��邩
	/// </summary>
	/// <param name=""></param>
	/// <returns> �I��true�A���s��false </returns>
	bool isTaskEnd(void);

	/// <summary>
	/// �^�X�N���I������܂ő҂�
	/// </summary>
	/// <param name=""></param>
	void Wait(void);
private:

	/// <summary>
	/// ���s�p�����֐�(�X���b�h�Ŏ��s)
	/// </summary>
	/// <param name=""></param>
	void Run(void);

	// �^�X�N(����������)
	std::function<void(void)> task_;

	// ���s���邩�̃t���O
	std::atomic_bool isRun_;

	// �^�X�N���I���������̃t���O
	std::atomic_bool isTaskEnd_;

	// �~���[�e�b�N�X
	std::mutex mutex_;

	// �����ϐ�
	std::condition_variable cd_;

	// �X���b�h�v�[���N���X�̎Q��
	ThreadPool& threadPool_;

	// ��Ɨp�X���b�h
	std::thread workerThread_;
};

