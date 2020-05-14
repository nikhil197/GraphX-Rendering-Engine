#pragma once

#include "Event.h"

namespace GraphX
{
	/* Abstract event for key events */
	class KeyEvent 
		: public Event
	{
	protected:
		/* Key code of the key pressed */
		int m_KeyCode;

		KeyEvent(int keyCode)
		{
			if (keyCode < 100)
				m_KeyCode = keyCode;
			else if (keyCode > 250 && keyCode < 320)
				m_KeyCode = keyCode - 150;
			else if (keyCode >= 320)
				m_KeyCode = keyCode - 320;
			else
				m_KeyCode = -1;
		}

	public:
		/* Returns the key code */
		inline int GetKeyCode() const { return m_KeyCode; }

		/* Category of the event type */
		EVENT_CLASS_CATEGORY(EventCategory::GX_EVENT_CATEGORY_KEYBOARD | EventCategory::GX_EVENT_CATEGORY_INPUT)
	};

	class KeyPressedEvent
		: public KeyEvent
	{
	private:
		/* Number of times the key is repeated */
		int m_RepeatCount;

	public:
		KeyPressedEvent(int keyCode, int repeatCount) 
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		/* Returns the repeat count for the key */
		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " times repeated)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(GX_KEY_PRESSED)
	};

	class KeyReleasedEvent
		: public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode) 
			: KeyEvent(keyCode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(GX_KEY_RELEASED)
	};
}