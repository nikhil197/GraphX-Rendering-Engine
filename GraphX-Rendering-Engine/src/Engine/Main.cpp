#include "pch.h"

#include "Application.h"

int main()
{
	//Title of the application
	std::string title = "GraphX Rendering Engine";
	
	GX_PROFILER_BEGIN_SESSION("Application::Initialise", "GraphX-Initialise.json")
	GraphX::Application* app = new GraphX::Application(title, 1920, 1080);
	GX_PROFILER_END_SESISON()

		// Run the application
	GraphX::Profiler::Get().BeginSession("Application::Run", "GraphX-Run.json");
	app->Run();
	GraphX::Profiler::Get().EndSession();

	GX_PROFILER_BEGIN_SESSION("Application::Shutdown", "GraphX-Shutdown.json")
	delete app;
	GX_PROFILER_END_SESISON()

	return 0;
}