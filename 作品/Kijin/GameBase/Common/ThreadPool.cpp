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

// 非同期で実行したい関数と終了時のコールバック関数を追加する
void ThreadPool::Add(std::function<void(void)>&& func)
{
	// 今テストでスレッドで処理していない
	func();
	return;

	// ロックする
	std::lock_guard<std::mutex> lock(mutex_);

	// リストに追加
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


// 処理中ののみ待つ
void ThreadPool::Wait(void)
{
	for (auto& w : workers_)
	{
		w->Wait();
	}
}

// すべてのタスクを待つ
//void ThreadPool::WaitAllTask(void)
//{
//	std::atomic_bool flag;
//	flag.store(true);
//	Add([&flag]() { flag.store(false); });
//
//	// 上記の処理が終わるまで待つ
//	while (flag.load())
//	{
//		// このスレッドがCPUを占有しないようにする
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

// 実行
//void ThreadPool::Run(void)
//{
//	std::function<void(void)>  func;
//	while (runFlag_.load())
//	{
//		{
//			std::unique_lock<std::mutex> lock(mutex_);
//
//			// リストの中身が入るまで待機
//			cd_.wait(lock, [this]() { return taskList_.size() >= 1; });
//
//			// 入ったらリストから取り出す
//			func = std::move(*taskList_.begin());
//			taskList_.pop_front();
//		}
//
//		// 実行する
//		func();
//	}
//}
