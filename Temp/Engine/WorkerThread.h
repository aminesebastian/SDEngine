#pragma once
#include <thread>
#include <functional>

using namespace std;

typedef std::thread SThread;
typedef std::thread::id SThreadID;

class WorkerThread {
public:
	WorkerThread(std::function<void()> ThreadFunction);
	~WorkerThread();

	void Wait();
	SThreadID GetID();

private:
	void workerFunc();

	std::function<void()> m_ThreadFunction;
	thread* m_Thread;
};

