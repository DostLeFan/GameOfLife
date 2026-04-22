#include "../include/RegularTask.hpp"

#include <chrono>

RegularTask::RegularTask(lluint delay) : m_delay(delay), m_isActive(false) {}

RegularTask::~RegularTask()
{
	stop();
}


void RegularTask::start()
{
	if(m_isActive.load() && m_thread.has_value())
		return;
	
	m_isActive.store(true);
	m_thread.emplace(&RegularTask::run, this);
}

void RegularTask::stop()
{
	m_isActive.store(false);
	m_stopCondition.notify_all();
	
	if(m_thread.has_value() && m_thread->joinable())
		m_thread->join();
	
	m_thread.reset();
}


void RegularTask::run()
{
	
	while(m_isActive)
	{
		process();
		
		std::unique_lock<std::mutex> lock(m_stopMutex);
		std::chrono::time_point<std::chrono::steady_clock> nextPause = std::chrono::steady_clock::now() + std::chrono::milliseconds(m_delay);

		// Thread will be stoped if m_isActive is set to false. Else, thread will just wait.
		m_stopCondition.wait_until(
			lock,
			nextPause,
			[this] {
				return !m_isActive;
			}
		);
	}
}