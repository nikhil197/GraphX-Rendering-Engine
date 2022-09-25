#pragma once

namespace GraphX
{
	// Base class for GraphX objects
	class GObject
	{
	private:
		// To keep track of the number of objects in the engine
		static long s_Objects;

	protected:
		GObject(const std::string& Name);

		GObject(std::string&& Name);

	public:
		/* Returns the name of the object */
		inline const std::string& GetName() const { return m_Name; }

	private:
		std::string m_Name;
	};
}