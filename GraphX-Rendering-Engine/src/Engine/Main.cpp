#include "pch.h"

#include "Application.h"

int main()
{
	//Title of the application
	std::string title = "GraphX Rendering Engine";
	GraphX::Application* app = new GraphX::Application(title, 1920, 1080);

	// Run the application
	app->Run();

	delete app;

	return 0;
}