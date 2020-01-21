#include "WorkerThread.h"

WorkerThread::WorkerThread(std::function<void()> ThreadFunction) {
	m_ThreadFunction	= ThreadFunction;
	m_Thread			= new thread(&WorkerThread::workerFunc, this);
}
WorkerThread::~WorkerThread() {

}

void WorkerThread::Wait() {
	m_Thread->join();
}
SThreadID WorkerThread::GetID() {
	return m_Thread->get_id();
}
void WorkerThread::workerFunc() {
	m_ThreadFunction();
}
