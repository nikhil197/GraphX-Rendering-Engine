#pragma once

namespace engine
{
	class Renderer3DCore
	{
	public:
		virtual void Submit(const class Mesh3D* mesh) = 0;

		virtual void Render() = 0;
	};
}