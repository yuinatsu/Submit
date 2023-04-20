#include "ThreadPool.h"
#include "Worker.h"
#include <deque>

#include "Debug.h"

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

// �񓯊��Ŏ��s�������֐���ǉ�����
void ThreadPool::Add(std::function<void(void)>&& func)
{
	// ���b�N����
	std::lock_guard<std::mutex> lock(mutex_);

	// ���X�g�ɒǉ�
	taskList_.push_back(std::move(func));

	for (auto& w : workers_)
	{
		if (w->isTaskEnd())
		{
			// �^�X�N�I�����Ă���worker��������^�X�N���Z�b�g����
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
		// worker��join����
		w->Join();
	}
}


// �������̂̂ݑ҂�
void ThreadPool::Wait(void)
{
	for (auto& w : workers_)
	{
		// worker�̏�����҂�
		w->Wait();
	}
}


void ThreadPool::WaitAllTask(void)
{
	while (true)
	{
		if (taskList_.empty())
		{
			// �^�X�N���X�g���k�ɂȂ����甲����
			break;
		}

		// ���X���b�h�ɏ����̋@���^����
		std::this_thread::yield();
	}

	// ���݂̏������̃^�X�N���I���܂ő҂�
	Wait();
}

bool ThreadPool::GetTask(Worker& worker)
{
	std::lock_guard<std::mutex> lock(mutex_);
	if (taskList_.empty())
	{
		// �^�X�N���Ȃ��Ƃ�
		return false;
	}

	// �^�X�N���Z�b�g����
	worker.SetTask(std::move(*taskList_.begin()));
	taskList_.pop_front();
	DebugLog("�Q�b�g");
	return true;
}

