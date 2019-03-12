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

		/* Submit the model to be rendered */
		virtual void Submit(const class Model3D* model) override;

		/* Submit the terrain for rendering */
		virtual void Submit(const class Terrain* terrain) override;

		/* Renders the objects submitted to the rendered*/
		virtual void Render() override;

		/* Renders the objects submitted to the renderer to the depth framebuffer (Shader should be bound before calling the render method) */
		virtual void Render(class Shader& DepthShader) override;
	};
}