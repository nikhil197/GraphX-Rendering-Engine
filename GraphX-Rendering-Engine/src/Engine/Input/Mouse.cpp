#include "pch.h"

#include "Mouse.h"
#include "Events/MouseEvent.h"

namespace GraphX
{
	std::shared_ptr<Mouse> Mouse::s_Mouse = nullptr;

	Mouse::Mouse()
		: m_LeftButtonPressed(0), m_RightButtonPressed(0), m_MiddleButtonPressed(0), Sensitivity(1.0f), ScrollSenstivity(0.1f), m_Position(0), m_PositionDelta(0), m_ScrollOffset(0)
	{}

	void Mouse::Init()
	{
		s_Mouse.reset(new Mouse());
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
		m_PositionDelta.x = (e.GetX() - m_Position.x) * Sensitivity;
		m_PositionDelta.y = (e.GetY() - m_Position.y) * Sensitivity;
		
		m_Position.x = e.GetX();
		m_Position.y = e.GetY();
	}

	void Mouse::OnEvent(MouseScrolledEvent& e)
	{
		m_ScrollOffset.x = e.GetXOffset() * ScrollSenstivity;
		m_ScrollOffset.y = e.GetYOffset() * ScrollSenstivity;
	}

	void Mouse::Update()
	{
		// Set the position delta and scroll offset back to zero, the event must be handled by the application before updating the mouse
		m_PositionDelta = GM::Vector2::ZeroVector;
		m_ScrollOffset = GM::Vector2::ZeroVector;
	}
}