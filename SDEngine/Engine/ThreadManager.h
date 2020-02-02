#pragma once
#include "WorkerThread.h"
#include "Core/DataTypes/TypeDefenitions.h"


class ThreadManager {
public:
	ThreadManager();
	~ThreadManager();


private:
	void* m_ThreadFunction;
	SSet<WorkerThread*> m_ActiveThreads;
};

