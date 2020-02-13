#pragma once

#include "Renderer2DCore.h"

namespace GraphX
{
	class Mesh2D;
	class Shader;

	class Renderer2D
		: public Renderer2DCore
	{
		friend class Renderer;
	private:
		/* Renderer Class provides abstraction for Renderer. No instantiation required */
		Renderer2D() = default;

	public:
		/* Submit the mesh to be rendered to the render */
		virtual void Submit(const Ref<Mesh2D>& mesh) override;

		/* Renders the objects submitted to the renderer */
		virtual void Render() override;

		/* Renders the objects submitted to the renderer to the depth framebuffer (Shader should be bound before calling the render method) */
		virtual void Render(Shader& DepthShader) override;

	private:
		/* Queue containing the objects to be rendered */
		std::deque<Ref<Mesh2D>> m_RenderQueue;
	};
}