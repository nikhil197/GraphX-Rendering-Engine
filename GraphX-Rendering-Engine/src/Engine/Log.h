#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace GraphX
{
	class Log
	{
	public:
		/* Initialize the logger */
		static void Init();

		/* Returns the logger */
		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return s_EngineLogger; }

		/* Returns the client application logger */
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		/* Shared Ptr to the logger */
		static std::shared_ptr<spdlog::logger> s_EngineLogger;

		/* Logger for the client applications */
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Macros to use the logger
#if GX_LOGGING
	#define GX_ENGINE_WARN(...)		::GraphX::Log::GetEngineLogger()->warn(__VA_ARGS__)
	#define GX_ENGINE_INFO(...)		::GraphX::Log::GetEngineLogger()->info(__VA_ARGS__)
	#define GX_ENGINE_TRACE(...)	::GraphX::Log::GetEngineLogger()->trace(__VA_ARGS__)
	#define GX_ENGINE_ERROR(...)	::GraphX::Log::GetEngineLogger()->error(__VA_ARGS__)
	#define GX_ENGINE_CRITICAL(...) ::GraphX::Log::GetEngineLogger()->critical(__VA_ARGS__)

	#define GX_CLIENT_WARN(...)		::GraphX::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define GX_CLIENT_INFO(...)		::GraphX::Log::GetClientLogger()->info(__VA_ARGS__)
	#define GX_CLIENT_TRACE(...)	::GraphX::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define GX_CLIENT_ERROR(...)	::GraphX::Log::GetClientLogger()->error(__VA_ARGS__)
#else
	#define GX_ENGINE_WARN(...) 
	#define GX_ENGINE_INFO(...) 
	#define GX_ENGINE_TRACE(...)
	#define GX_ENGINE_ERROR(...)
	#define GX_ENGINE_CRITICAL(...)

	#define GX_CLIENT_WARN(...)		
	#define GX_CLIENT_INFO(...)		
	#define GX_CLIENT_TRACE(...)	
	#define GX_CLIENT_ERROR(...)
#endif