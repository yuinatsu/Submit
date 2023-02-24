#include "Worker.h"
#include "ThreadPool.h"

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
		isRun_.store(false);

		if (isTaskEnd_.load())
		{
			SetTask([]() {});
		}
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
		std::this_thread::yield();
	}

}

void Worker::Run(void)
{
	while (isRun_.load())
	{
		{
			std::unique_lock<std::mutex> lock(mutex_);

			// リストの中身が入るまで待機
			cd_.wait(lock, [this]() { return task_; });

			isTaskEnd_.store(false);

			// 実行する
			task_();

			isTaskEnd_.store(true);
		}
		threadPool_.GetTask(*this);
		
	}
}
