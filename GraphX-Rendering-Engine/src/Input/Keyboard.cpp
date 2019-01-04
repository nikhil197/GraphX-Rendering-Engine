#include "pch.h"

#include "Keyboard.h"
#include "Events/KeyboardEvent.h"

namespace engine
{
	std::shared_ptr<Keyboard> Keyboard::s_Keyboard;

	Keyboard::Keyboard(ConstructorHelper&& ch)
	{
		constexpr int n = Keys::GX_F12;
		m_Keys = new std::vector<bool>(n);
	}

	void Keyboard::Init()
	{
		s_Keyboard = std::make_shared<Keyboard>(ConstructorHelper());
	}

	void Keyboard::OnEvent(KeyPressedEvent& e)
	{
		(*m_Keys)[e.GetKeyCode()] = true;
	}

	void Keyboard::OnEvent(KeyReleasedEvent& e)
	{
		(*m_Keys)[e.GetKeyCode()] = false;
	}

	Keyboard::~Keyboard()
	{
		delete m_Keys;
	}
}