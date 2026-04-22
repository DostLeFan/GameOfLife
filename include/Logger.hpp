#ifndef DEF_LOGGER
#define DEF_LOGGER

#ifndef NOMINMAX
    #define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include <memory>
#include <mutex>
#include <spdlog/async.h>
#include <spdlog/spdlog.h>

class Logger
{
	public:
		static void init();
		inline static std::shared_ptr<spdlog::logger>& get() { return m_logger; }

	private:
		static std::shared_ptr<spdlog::logger> m_logger;
		static std::once_flag m_initFlag;
};

#define LOG_TRACE(...)    do { if (Logger::get()) SPDLOG_LOGGER_TRACE(Logger::get(), __VA_ARGS__); } while(0)
#define LOG_DEBUG(...)    do { if (Logger::get()) SPDLOG_LOGGER_DEBUG(Logger::get(), __VA_ARGS__); } while(0)
#define LOG_INFO(...)     do { if (Logger::get()) SPDLOG_LOGGER_INFO(Logger::get(), __VA_ARGS__); } while(0)
#define LOG_WARN(...)     do { if (Logger::get()) SPDLOG_LOGGER_WARN(Logger::get(), __VA_ARGS__); } while(0)
#define LOG_ERROR(...)    do { if (Logger::get()) SPDLOG_LOGGER_ERROR(Logger::get(), __VA_ARGS__); } while(0)
#define LOG_CRITICAL(...) do { if (Logger::get()) SPDLOG_LOGGER_CRITICAL(Logger::get(), __VA_ARGS__); } while(0)

#endif // DEF_LOGGER