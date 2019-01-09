#include "pch.h"

#include "Mouse.h"
#include "Events/MouseEvent.h"

namespace engine
{
	std::shared_ptr<Mouse> Mouse::s_Mouse;

	Mouse::Mouse(ConstructorHelper&& ch)
		: m_LeftButtonPressed(0), m_RightButtonPressed(0), m_MiddleButtonPressed(0), m_Senstivity(0.1f), m_Position(0), m_LastPosition(0)
	{}

	void Mouse::Init()
	{
		s_Mouse = std::make_shared<Mouse>(ConstructorHelper());
	}

	void Mouse::OnEvent(MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == MouseButton::GX_MOUSE_LEFT)
			m_LeftButtonPressed = 1;
		else if (e.GetButton() == MouseButton::GX_MOUSE_RIGHT)
			m_RightButtonPressed = 1;
		else if (e.GetButton() == MouseButton::GX_MOUSE_MIDDLE)
			m_MiddleButtonPressed = 1;
	}

	void Mouse::OnEvent(MouseButtonReleasedEvent& e)
	{
		if (e.GetButton() == MouseButton::GX_MOUSE_LEFT)
			m_LeftButtonPressed = 0;
		else if (e.GetButton() == MouseButton::GX_MOUSE_RIGHT)
			m_RightButtonPressed = 0;
		else if (e.GetButton() == MouseButton::GX_MOUSE_MIDDLE)
			m_MiddleButtonPressed = 0;
	}

	void Mouse::OnEvent(MouseMovedEvent& e)
	{
		m_LastPosition = m_Position;
		float xOffset = e.GetX() - m_Position.x;
		float yOffset = e.GetY() - m_Position.y;

		xOffset *= m_Senstivity;
		yOffset *= m_Senstivity;

		m_Position.x += xOffset;
		m_Position.y += yOffset;
	}

	void Mouse::Update()
	{
		// Update the last mouse position to the current at the end of every frame
		m_LastPosition = m_Position;
	}
}