#pragma once

#include "Renderer3DCore.h"

namespace GraphX
{
	class Renderer3D
		: public Renderer3DCore
	{
		friend class Renderer;
	private:
		/* Renderer Class provides abstraction for Renderer. No instantiation required */
		Renderer3D() = default;

	public:
		/* Submit the object to be rendered */
		virtual void Submit(const Ref<Mesh3D>& mesh) override;

		/* Submit the model to be rendered */
		virtual void Submit(const Ref<Model3D>& model) override;

		/* Submit the terrain for rendering */
		virtual void Submit(const Ref<Terrain>& terrain) override;

		/* Renders the objects submitted to the rendered*/
		virtual void Render() override;

		/* Renders the objects submitted to the renderer to the depth framebuffer (Shader should be bound before calling the render method) */
		virtual void Render(Shader& DepthShader) override;

		/* Renders the collision bounds for debugging */
		virtual void RenderDebugCollisions(const Ref<GM::BoundingBox>& Box, const GM::Matrix4& Model) override;

	private:
		/* Queue containing the objects to be rendered */
		std::deque<Ref<Mesh3D>> m_RenderQueue;
	};
}