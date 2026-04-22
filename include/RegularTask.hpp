#ifndef DEF_REGULARTASK
#define DEF_REGULARTASK

#include <optional>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

typedef long long unsigned int lluint;

class RegularTask
{
	public:
		RegularTask(lluint delay);
		RegularTask(RegularTask const& src) = delete;
		~RegularTask();
		
		RegularTask& operator=(RegularTask const& src) = delete;
		
		inline lluint getDelay() const { return m_delay; }
		inline void setDelay(lluint delay) { m_delay = delay; }
		
		void start();
		virtual void stop();
		inline bool isActive() const { return m_isActive; }
	
	private:
		lluint m_delay;
		std::atomic<bool> m_isActive;
		std::optional<std::thread> m_thread;
		std::mutex m_stopMutex;
		std::condition_variable m_stopCondition;
		
		void run();
	
	protected:
		virtual void process() = 0;
};

#endif // DEF_REGULARTASK