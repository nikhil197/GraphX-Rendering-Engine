#pragma once
#include "pch.h"

namespace GraphX
{
	template<typename T>
	class MemoryManager
	{
	private:
		/* Collection of the objects created in the engine */
		std::vector<T*> m_Objects;

	public:
		/* Create a new object of the specified template type */
		void AddObject(T* Object);

		/* Removes an object */
		bool RemoveObject(T* Object);

		~MemoryManager();
	};
}

// Implementation of the Memory Manager
#include "MemoryManager.inl"