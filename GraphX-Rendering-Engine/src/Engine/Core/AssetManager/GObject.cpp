#include "pch.h"
#include "GObject.h"

namespace GraphX
{
	long GObject::s_Objects;

	GObject::GObject(const std::string& Name)
		: m_Name(Name)
	{
		s_Objects++;
	}

	GObject::GObject(std::string&& Name)
		:m_Name(std::move(Name))
	{
		s_Objects++;
	}
}