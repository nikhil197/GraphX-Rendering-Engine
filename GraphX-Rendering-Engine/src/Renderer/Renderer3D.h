#pragma once

#include "Renderer3DCore.h"

namespace engine
{
	class Renderer3D
		: public Renderer3DCore
	{
	private:
		/* Queue containing the objects to be rendered */
		std::deque<const class Mesh3D*> m_RenderQueue;

	public:
		/* Submit the object to be rendered */
		virtual void Submit(const class Mesh3D* mesh) override;

		/* Renders the objects submitted to the rendered*/
		virtual void Render() override;
	};
}