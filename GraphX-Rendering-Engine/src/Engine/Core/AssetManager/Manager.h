#pragma once

// For handling GraphX Assets (Like Textures, shaders, materials, etc.)
namespace GraphX
{
	// NOTE: To Be Replaced the Engine's Asset Manager
	// A Scoped (Unique) Pointer
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	// A Shared Pointer
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	constexpr Ref<T> CreateRef(T* Pointer)
	{
		return std::shared_ptr<T>(Pointer);
	}
}