#pragma once

#include "Event.h"
#include "Model/ModelTypes.h"

namespace engine
{
	class AddTextureEvent
		: public Event
	{
	public:
		AddTextureEvent() {}

		EVENT_CLASS_TYPE(GX_ADD_TEXTURE)
		EVENT_CLASS_CATEGORY(GX_EVENT_CATEGORY_GUI)
	};

	class AddModelEvent
		: public Event
	{
	private:
		/* Model Type to be added */
		ModelType m_ModelType;

	public:
		AddModelEvent(ModelType type)
			: Event(), m_ModelType(type)
		{}

		/* Returns the type of the model */
		inline ModelType GetModelType() const { return m_ModelType; }

		EVENT_CLASS_TYPE(GX_LOAD_MODEL)
		EVENT_CLASS_CATEGORY(GX_EVENT_CATEGORY_GUI)
	};
}
