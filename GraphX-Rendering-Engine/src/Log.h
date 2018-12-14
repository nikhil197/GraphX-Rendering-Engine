#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace engine
{
	class Log
	{
	private:
		/* Shared Ptr to the logger */
		static std::shared_ptr<spdlog::logger> s_EngineLogger;

	public:
		/* Initialize the logger */
		static void Init();

		/* Returns the logger */
		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }
	};
}

// Macros to use the logger
#define GX_ENGINE_WARN(...)  ::engine::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define GX_ENGINE_INFO(...)  ::engine::Log::GetEngineLogger()->info(__VA_ARGS__)
#define GX_ENGINE_TRACE(...) ::engine::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define GX_ENGINE_ERROR(...) ::engine::Log::GetEngineLogger()->error(__VA_ARGS__)