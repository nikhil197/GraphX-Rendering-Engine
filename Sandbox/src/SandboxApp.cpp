
//#include "pch.h"
#include "GraphX.h"
#include "EntryPoint.h"

class SandboxApp
	: public GraphX::Application
{
public:
	SandboxApp()
		: Application("GraphX-Rendering-Engine", 1920, 1080)
	{

	}
};

GraphX::Application* GraphX::CreateApplication()
{
	return new SandboxApp();
}