#pragma once

#include "Event.h"

namespace engine
{
	class WindowCloseEvent
		: public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(GX_WINDOW_CLOSED)
		EVENT_CLASS_CATEGORY(GX_EVENT_CATEGORY_WINDOW)
	};

	class WindowResizedEvent
		: public Event
	{
	private:
		/* New width and height of the window */
		unsigned int m_Width, m_Height;

	public:
		WindowResizedEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		/* Returns the width of the window */
		inline unsigned int GetWidth() const { return m_Width; }

		/* Returns the height of the window */
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizedEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(GX_WINDOW_RESIZED)
		EVENT_CLASS_CATEGORY(GX_EVENT_CATEGORY_WINDOW)
	};

	class WindowMovedEvent
		: public Event
	{
	private:
		/* New width and height of the window */
		int m_XPos, m_YPos;

	public:
		WindowMovedEvent(int x, int y)
			: m_XPos(x), m_YPos(y) {}

		/* Returns the width of the window */
		inline unsigned int GetWidth() const { return m_XPos; }

		/* Returns the height of the window */
		inline unsigned int GetHeight() const { return m_YPos; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_XPos << ", " << m_YPos;
			return ss.str();
		}

		EVENT_CLASS_TYPE(GX_WINDOW_MOVED)
		EVENT_CLASS_CATEGORY(GX_EVENT_CATEGORY_WINDOW)
	};

	class WindowFocusEvent
		: public Event
	{
	public:
		WindowFocusEvent() {}

		EVENT_CLASS_TYPE(GX_WINDOW_FOCUS)
		EVENT_CLASS_CATEGORY(GX_EVENT_CATEGORY_WINDOW)
	};

	class WindowLostFocusEvent
		: public Event
	{
	public:
		WindowLostFocusEvent() {}

		EVENT_CLASS_TYPE(GX_WINDOW_LOST_FOCUS)
		EVENT_CLASS_CATEGORY(GX_EVENT_CATEGORY_WINDOW)
	};
}