#pragma once

#include "Engine/Log.h"
#include "Engine/Profiler/Profiler.h"

extern GraphX::Application* GraphX::CreateApplication();

int main(int argc, char** argv)
{
	//Title of the application
	const char* title = "GraphX Rendering Engine";

	GX_PROFILER_BEGIN_SESSION("Application::Initialise", "logs/Profiling/GraphX-Initialise.json")
	GraphX::Application* app = GraphX::CreateApplication();
	GX_PROFILER_END_SESISON()

	// Run the application
	GX_PROFILER_BEGIN_SESSION("Application::Run", "logs/Profiling/GraphX-Run.json");
	app->Run();
	GX_PROFILER_END_SESISON();

	GX_PROFILER_BEGIN_SESSION("Application::Shutdown", "logs/Profiling/GraphX-Shutdown.json")
	delete app;
	GX_PROFILER_END_SESISON()
}