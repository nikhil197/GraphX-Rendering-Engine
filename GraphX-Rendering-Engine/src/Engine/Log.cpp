#include "pch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace GraphX
{
	std::shared_ptr <spdlog::logger> Log::s_EngineLogger;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> loggerSinks;
		
		// Console Sink
		loggerSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		loggerSinks[0]->set_pattern("%^[%T] %n: %v%$");

		// File Sink
		loggerSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("GraphX.log", true));
		loggerSinks[1]->set_pattern("[%T] [%l]%n: %v");

		s_EngineLogger = std::make_shared<spdlog::logger>("GX-Engine", loggerSinks.begin(), loggerSinks.end());
		s_EngineLogger->set_level(spdlog::level::trace);
	}
}