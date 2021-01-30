#pragma once

#include "GraphX.h"

class SandboxApp
	: public GraphX::Application
{
public:
	SandboxApp()
		: Application("GraphX-Rendering-Engine", 1920, 1080)
	{}
};