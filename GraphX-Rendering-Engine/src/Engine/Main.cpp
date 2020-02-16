#include "pch.h"

#include "Application.h"

int main()
{
	//Title of the application
	std::string title = "GraphX Rendering Engine";
	GraphX::Application* app = new GraphX::Application(title, 1920, 1080);

	// Run the application
	GX_PROFILER_BEGIN_SESSION("Application::Run", "GraphX-Run.json")
	app->Run();
	GX_PROFILER_END_SESISON()

	delete app;

	return 0;
}