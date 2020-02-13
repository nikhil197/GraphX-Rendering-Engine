#pragma once

namespace GraphX
{
	class Mesh3D;
	class Model3D;
	class Terrain;
	class Shader;

	class Renderer3DCore
	{
	public:
		virtual void Submit(const Ref<Mesh3D>& mesh) = 0;

		virtual void Submit(const Ref<Model3D>& model) = 0;

		virtual void Submit(const Ref<Terrain>& terrain) = 0;

		virtual void Render() = 0;

		virtual void Render(class Shader& DepthShader) = 0;

		virtual void RenderDebugCollisions(const Ref<GM::BoundingBox>& Box, const GM::Matrix4& Model) = 0;
	};
}