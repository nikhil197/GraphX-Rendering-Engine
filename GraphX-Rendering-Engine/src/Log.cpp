#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace engine
{
	std::shared_ptr <spdlog::logger> Log::s_EngineLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_EngineLogger = spdlog::stdout_color_mt("GX-Engine");
		s_EngineLogger->set_level(spdlog::level::trace);
	}
}