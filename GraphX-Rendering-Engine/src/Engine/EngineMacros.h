#pragma once

#define GX_ENGINE_ASSERT(x, ...) { if(!(x)) { GX_ENGINE_CRITICAL("[Assertion Failed] {0}", #__VA_ARGS__); __debugbreak(); } }

// Macro to bind the class event functions
#define BIND_EVENT_FUNC(x) std::bind(&x, this, std::placeholders::_1)

#define BIT(x) (1 << x)