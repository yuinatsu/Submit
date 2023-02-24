#pragma once
#include <thread>
#include <functional>
#include <atomic>
#include <condition_variable>
#include <mutex>

class ThreadPool;

class Worker
{
public:
	Worker(ThreadPool& threadPool);
	~Worker();
	void SetTask(std::function<void(void)>&& task);
	void Stop(void);
	void Join(void);
	bool isTaskEnd(void);
	void Wait(void);
private:
	void Run(void);
	std::function<void(void)> task_;
	std::atomic_bool isRun_;
	std::atomic_bool isTaskEnd_;
	std::mutex mutex_;
	std::condition_variable cd_;
	ThreadPool& threadPool_;
	std::thread workerThread_;
};

