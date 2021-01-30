#pragma once

#include "Event.h"

namespace GraphX
{
	class MouseMovedEvent
		: public Event
	{
	private:
		/* Current X & Y position */
		float m_MouseX, m_MouseY;

	public:
		MouseMovedEvent(float x, float y) 
			: m_MouseX(x), m_MouseY(y) {}

		/* Returns the X - position of the mouse pointer */
		inline float GetX() const { return m_MouseX; }

		/* Returns the Y - position of the mouse pointer */
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(GX_MOUSE_MOVED)
		EVENT_CLASS_CATEGORY(EventCategory::GX_EVENT_CATEGORY_MOUSE | EventCategory::GX_EVENT_CATEGORY_INPUT)
	};

	class MouseScrolledEvent
		: public Event
	{
	private:
		/* Amount scrolled in each direction */
		float m_XOffset, m_YOffset;

	public:
		MouseScrolledEvent(float x, float y)
			: m_XOffset(x), m_YOffset(y) {}

		inline float GetXOffset() const { return m_XOffset; }

		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_XOffset << ", " << m_YOffset;
			return ss.str();
		}

		EVENT_CLASS_TYPE(GX_MOUSE_SCROLLED)
		EVENT_CLASS_CATEGORY(EventCategory::GX_EVENT_CATEGORY_MOUSE | EventCategory::GX_EVENT_CATEGORY_INPUT)
	};

	class MouseButtonEvent
		: public Event
	{
	protected:
		/* Mouse Button pressed */
		int m_Button;

		MouseButtonEvent(int button)
			:m_Button(button) {}

	public:
		/* Returns the button pressed */
		inline int GetButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategory::GX_EVENT_CATEGORY_MOUSE | EventCategory::GX_EVENT_CATEGORY_MOUSE_BUTTON | EventCategory::GX_EVENT_CATEGORY_INPUT)
	};

	class MouseButtonPressedEvent
		: public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(GX_MOUSE_BUTTON_PRESSED)
	};

	class MouseButtonReleasedEvent
		: public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(GX_MOUSE_BUTTON_RELEASED)
	};
}