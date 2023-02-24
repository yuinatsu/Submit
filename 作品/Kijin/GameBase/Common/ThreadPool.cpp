#include "ThreadPool.h"
#include "Worker.h"
#include <deque>

ThreadPool::ThreadPool(unsigned int threadNum)
{
	workers_.resize(threadNum);
	for (auto& w : workers_)
	{
		w = std::make_unique<Worker>(*this);
	}
}

ThreadPool::~ThreadPool()
{
	Join();
}

// �񓯊��Ŏ��s�������֐��ƏI�����̃R�[���o�b�N�֐���ǉ�����
void ThreadPool::Add(std::function<void(void)>&& func)
{
	// ���e�X�g�ŃX���b�h�ŏ������Ă��Ȃ�
	func();
	return;

	// ���b�N����
	std::lock_guard<std::mutex> lock(mutex_);

	// ���X�g�ɒǉ�
	taskList_.push_back(std::move(func));

	for (auto& w : workers_)
	{
		if (w->isTaskEnd())
		{
			w->SetTask(std::move(*taskList_.begin()));
			taskList_.pop_front();
			return;
		}
	}

}

void ThreadPool::Join(void)
{
	for (auto& w : workers_)
	{
		w->Join();
	}
}


// �������̂̂ݑ҂�
void ThreadPool::Wait(void)
{
	for (auto& w : workers_)
	{
		w->Wait();
	}
}

// ���ׂẴ^�X�N��҂�
//void ThreadPool::WaitAllTask(void)
//{
//	std::atomic_bool flag;
//	flag.store(true);
//	Add([&flag]() { flag.store(false); });
//
//	// ��L�̏������I���܂ő҂�
//	while (flag.load())
//	{
//		// ���̃X���b�h��CPU���L���Ȃ��悤�ɂ���
//		std::this_thread::yield();
//	}
//}

void ThreadPool::WaitAllTask(void)
{
	while (true)
	{
		if (taskList_.empty())
		{
			break;
		}
		std::this_thread::yield();
	}
	Wait();
}

void ThreadPool::GetTask(Worker& worker)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (taskList_.empty())
	{
		return;
	}
	worker.SetTask(std::move(*taskList_.begin()));
	taskList_.pop_front();
}

// ���s
//void ThreadPool::Run(void)
//{
//	std::function<void(void)>  func;
//	while (runFlag_.load())
//	{
//		{
//			std::unique_lock<std::mutex> lock(mutex_);
//
//			// ���X�g�̒��g������܂őҋ@
//			cd_.wait(lock, [this]() { return taskList_.size() >= 1; });
//
//			// �������烊�X�g������o��
//			func = std::move(*taskList_.begin());
//			taskList_.pop_front();
//		}
//
//		// ���s����
//		func();
//	}
//}
