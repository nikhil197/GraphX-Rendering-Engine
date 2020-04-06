#include "pch.h"

#include "Application.h"

int main()
{
	//Title of the application
	const char* title = "GraphX Rendering Engine";
	
	GX_PROFILER_BEGIN_SESSION("Application::Initialise", "logs/Profiling/GraphX-Initialise.json")
	GraphX::Application* app = new GraphX::Application(title, 1920, 1080);
	GX_PROFILER_END_SESISON()

		// Run the application
	GX_PROFILER_BEGIN_SESSION("Application::Run", "logs/Profiling/GraphX-Run.json");
	app->Run();
	GX_PROFILER_END_SESISON();

	GX_PROFILER_BEGIN_SESSION("Application::Shutdown", "logs/Profiling/GraphX-Shutdown.json")
	delete app;
	GX_PROFILER_END_SESISON()

	return 0;
}