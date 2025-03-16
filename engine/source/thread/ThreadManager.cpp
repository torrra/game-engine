#include "engine/thread/ThreadManager.h"


void engine::ThreadManager::Shutdown(void)
{
	// lock mutex here, we want
	// the bool to be assigned BEFORE
	// notify_all is called, or else threads
	// will be unable to join
	m_poolMutex.lock();
	m_stopThreads = true;
	m_poolMutex.unlock();

	m_conditionVariable.notify_all();

	for (std::thread& thread : m_workers)
		thread.join();

}

void engine::ThreadManager::TreadLoop(void)
{
	while (true)
	{
		// Define variable outside of block, so it can be executed after
		// lock releases mutex
		std::function<void()> task;

		// Add scope so lock releases when exiting this block
		{
			// lock for condition variable
			std::unique_lock lock(m_poolMutex);
			m_conditionVariable.wait(lock, [this]() {return m_stopThreads || !m_tasks.empty(); });

			// Only break when Shutdown was called AND all tasks are
			// completed. Otherwise the packaged_task pointers won't be be deleted and leak
			if (m_stopThreads && m_tasks.empty())
				break;

			// Exit this block (unlock mutex) and go back to sleep if no work is available and
			// thread is not supposed to join yet
			else if (m_tasks.empty())
				continue;

			task = m_tasks.front();
			m_tasks.pop();
		}

		// Run anonymous function that contains the packaged task ptr
		// and the delete statement
		task();
	}
}