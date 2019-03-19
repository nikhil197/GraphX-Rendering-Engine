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

	class CreateTerrainEvent
		: public Event
	{
	private:
		/* Terrain Object */
		class Terrain* m_Terrain;
	public:
		CreateTerrainEvent(Terrain* terrain)
			: m_Terrain(terrain)
		{}

		/* Returns the terrain */
		inline class Terrain* GetTerrain() const { return m_Terrain; }

		EVENT_CLASS_TYPE(GX_CREATE_TERRAIN)
		EVENT_CLASS_CATEGORY(GX_EVENT_CATEGORY_GUI)
	};

	/* Entity changed events */
	class EntityChangedEvent
		: public Event
	{
	protected:
		const class Entity& m_Entity;

	public:
		EntityChangedEvent(const Entity& e)
			: m_Entity(e)
		{}

		/* Returns the entity */
		virtual const class Entity& GetEntity() const = 0;

		EVENT_CLASS_TYPE(GX_ENTITY_CHANGED)
		EVENT_CLASS_CATEGORY(GX_EVENT_CATEGORY_GUI)
	};

	class CameraFOVChangedEvent
		: public EntityChangedEvent
	{
	public:
		CameraFOVChangedEvent(const Camera& cam)
			: EntityChangedEvent(cam)
		{}

		virtual const class Camera& GetEntity() const override { return *((Camera*)&m_Entity); }
	};
}
