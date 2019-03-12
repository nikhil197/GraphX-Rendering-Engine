#pragma once

#include "Renderer2DCore.h"

namespace engine
{
	class Renderer2D
		: public Renderer2DCore
	{
	private:
		/* Queue containing the objects to be rendered */
		std::deque<const class Mesh2D*> m_RenderQueue;

	public:
		/* Submit the mesh to be rendered to the render */
		virtual void Submit(const class Mesh2D& mesh) override;

		/* Renders the objects submitted to the renderer */
		virtual void Render() override;

		/* Renders the objects submitted to the renderer to the depth framebuffer (Shader should be bound before calling the render method) */
		virtual void Render(class Shader& DepthShader) override;
	};
}