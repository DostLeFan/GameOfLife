#include "../include/Logger.hpp"

#include <vector>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

std::shared_ptr<spdlog::logger> Logger::m_logger = nullptr;
std::once_flag Logger::m_initFlag;

void Logger::init()
{
	std::call_once(
		m_initFlag,
		[]() {
			spdlog::init_thread_pool(8192, 1);

			std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
			std::shared_ptr<spdlog::sinks::daily_file_sink_mt> dailySink = std::make_shared<spdlog::sinks::daily_file_sink_mt>("logs/daily.log", 0, 0);
			std::shared_ptr<spdlog::sinks::rotating_file_sink_mt> rotatingSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/rotating.log", 1024 * 1024 * 10, 5);

			std::vector<spdlog::sink_ptr> sinks = { consoleSink, dailySink, rotatingSink };

			m_logger = std::make_shared<spdlog::async_logger>("APP", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
			m_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");
			m_logger->set_level(spdlog::level::trace);

			spdlog::register_logger(m_logger);
			spdlog::set_default_logger(m_logger);
		}
	);
}