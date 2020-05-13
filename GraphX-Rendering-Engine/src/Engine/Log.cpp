#include "pch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace GraphX
{
	std::shared_ptr <spdlog::logger> Log::s_EngineLogger;

	std::shared_ptr <spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> loggerSinks;
		
		// Console Sink
		loggerSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		loggerSinks[0]->set_pattern("%^[%T] %n : %v%$");

		// File Sink
		loggerSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/GraphX.log", true));
		loggerSinks[1]->set_pattern("[%T] [%l] %n : %v");

		s_EngineLogger = std::make_shared<spdlog::logger>("GX-Engine", loggerSinks.begin(), loggerSinks.end());
		s_EngineLogger->set_level(spdlog::level::trace);
		s_EngineLogger->flush_on(spdlog::level::trace);

		loggerSinks[1].reset(new spdlog::sinks::basic_file_sink_mt("logs/Client.log", true));
		s_ClientLogger = std::make_shared<spdlog::logger>("GX-Client", loggerSinks.begin(), loggerSinks.end());
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
	}
}