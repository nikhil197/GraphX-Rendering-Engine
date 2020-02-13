#pragma once

namespace GraphX
{
	class Renderer2DCore
	{
	public:
		virtual void Submit(const Ref<class Mesh2D>& mesh) = 0;

		virtual void Render() = 0;

		virtual void Render(class Shader& DepthShader) = 0;
	};
}