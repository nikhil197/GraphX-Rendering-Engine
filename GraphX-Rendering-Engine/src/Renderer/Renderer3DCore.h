#pragma once

namespace GraphX
{
	class Renderer3DCore
	{
	public:
		virtual void Submit(const class Mesh3D* mesh) = 0;

		virtual void Submit(const class Model3D* model) = 0;

		virtual void Submit(const class Terrain* terrain) = 0;

		virtual void Render() = 0;

		virtual void Render(class Shader& DepthShader) = 0;
	};
}