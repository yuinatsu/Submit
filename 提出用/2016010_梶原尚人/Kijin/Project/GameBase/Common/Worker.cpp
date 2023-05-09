#include "Worker.h"
#include "ThreadPool.h"

#include "Debug.h"

Worker::Worker(ThreadPool& threadPool) :
	threadPool_{threadPool}
{
	isRun_.store(true);
	isTaskEnd_.store(true);
	workerThread_ = std::thread{ &Worker::Run,this };
}

Worker::~Worker()
{
	Join();
}

void Worker::SetTask(std::function<void(void)>&& task)
{
	std::lock_guard<std::mutex> lock(mutex_);
	task_ = std::move(task);
	isTaskEnd_.store(false);
	cd_.notify_all();
}

void Worker::Stop(void)
{
	isRun_.store(false);
}

void Worker::Join(void)
{
	if (workerThread_.joinable())
	{
		// �W���C���\�Ȏ����s�t���O��false��
		isRun_.store(false);

		if (isTaskEnd_.load())
		{
			// �^�X�N���I�����Ă���Ƃ��ҋ@�p�̃^�X�N������
			SetTask([]() {});
		}

		// join����
		workerThread_.join();
	}
}

bool Worker::isTaskEnd(void)
{
	return isTaskEnd_.load();
}

void Worker::Wait(void)
{
	while (!isTaskEnd_.load())
	{
		// �^�X�N���I������܂ł��̃X���b�h��yield���Ȃ���҂�
		std::this_thread::yield();
	}

}

void Worker::Run(void)
{
	while (isRun_.load())
	{
		{
			std::unique_lock<std::mutex> lock(mutex_);

			// ���X�g�̒��g������܂őҋ@(���ɓ����Ă�����i��)
			cd_.wait(lock, [this]() { return task_; });

			// ���s����
			task_();
			task_ = nullptr;
			isTaskEnd_.store(true);
		}

		// �^�X�N�擾�����݂�
		isTaskEnd_.store(!threadPool_.GetTask(*this));
		
	}
}
