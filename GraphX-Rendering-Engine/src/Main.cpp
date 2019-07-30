#include "pch.h"

#include "Application.h"

int main()
{
	//Title of the application
	std::string title = "GraphX Rendering Engine";
	std::unique_ptr<GraphX::Application> app = std::make_unique<GraphX::Application>(title, 1920, 1080);

	// Run the application
	app->Run();

	return 0;
}