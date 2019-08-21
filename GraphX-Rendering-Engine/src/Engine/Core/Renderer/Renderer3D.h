#pragma once

#include "Renderer3DCore.h"

namespace GraphX
{
	class Renderer3D
		: public Renderer3DCore
	{
		friend class Renderer;
	private:
		/* Queue containing the objects to be rendered */
		std::deque<const class Mesh3D*> m_RenderQueue;

	private:
		/* Renderer Class provides abstraction for Renderer. No instantiation required */
		Renderer3D() = default;

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

		/* Renders the collision bounds for debugging */
		virtual void RenderDebugCollisions(const GraphXMaths::BoundingBox* Box, const GraphXMaths::Matrix4& Model) override;
	};
}