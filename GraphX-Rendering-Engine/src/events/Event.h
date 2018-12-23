#pragma once

#include "pch.h"

namespace engine
{
#define BIT(x) (1 << x)

	// Currently GraphX Rendering engine only supports Blocking events.
	// Buffered events are further down the development pipeline.

	enum class EventType
	{
		NONE = 0,

		/* Mouse Events */
		GX_MOUSE_MOVED,
		GX_MOUSE_BUTTON_PRESSED,
		GX_MOUSE_BUTTON_RELEASED,
		GX_MOUSE_SCROLLED,

		/* Keyboard events */
		GX_KEY_PRESSED,
		GX_KEY_RELEASED,

		/* Window events */
		GX_WINDOW_CLOSED,
		GX_WINDOW_RESIZED,
		GX_WINDOW_MOVED,
		GX_WINDOW_FOCUS,
		GX_WINDOW_LOST_FOCUS
	};

	// Not a scoped enum because it doesn't represent any actual thing in the event, unlike event type
	enum EventCategory
	{
		NONE = 0,
		GX_EVENT_CATEGORY_INPUT			= BIT(0),
		GX_EVENT_CATEGORY_MOUSE			= BIT(1),
		GX_EVENT_CATEGORY_MOUSE_BUTTON  = BIT(2),
		GX_EVENT_CATEGORY_KEYBOARD		= BIT(3),
		GX_EVENT_CATEGORY_WINDOW		= BIT(4)
	};

	// Macros to override the base event class methods
#define EVENT_CLASS_TYPE(type)  static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	/* Event class */
	class Event
	{
		friend class EventDispatcher;

	public:
		/* Returns the static type of the event */
		static EventType GetStaticType() { return EventType::NONE; }

		/* Returns the type of the event */
		virtual EventType GetEventType() const = 0;

		/* Returns the name of the event (Possibly just for debugging) */
		virtual const char* GetName() const = 0;

		/* Returns the flags for all the different categories the event belongs to */
		virtual int GetCategoryFlags() const = 0;

		/* Returns a string containing info about the event (Possibly just for debugging) */
		virtual std::string ToString() const { return GetName(); }

		/* Returns if the event belongs to the specified category */
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	protected:
		/* Whether the event has been handled */
		bool m_Handled = false;
	};

	class EventDispatcher
	{
	private: 
		Event& m_Event;

	public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		/* Dispatches the event to the appropriate listener */
		template<typename T>
		bool Dispatch(std::function<bool(T&)> func)
		{
			if (!m_Event.m_Handled && m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	};

	static std::ostream& operator<<(std::ostream& out, const Event& event)
	{
		return out << event.ToString();
	}
}