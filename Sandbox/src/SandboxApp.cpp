#include "GraphX.h"
#include "EntryPoint.h"

#include "SandboxApp.h"

GraphX::Application* GraphX::CreateApplication()
{
	return new SandboxApp();
}