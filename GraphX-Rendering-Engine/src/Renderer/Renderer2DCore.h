#pragma once

namespace engine
{
	class Renderer2DCore
	{
	public:
		virtual void Submit(const class Mesh2D& mesh) = 0;

		virtual void Render() = 0;
	};
}