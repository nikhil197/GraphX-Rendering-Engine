#pragma once

namespace GraphX
{
	class SimpleRenderer
	{
		friend class Renderer;
	private:
		/* Renderer Class provides abstraction for Renderer. No instantiation required */
		SimpleRenderer() = default;

	public:
		/* Draws the Count number of indices (The object to be drawn must be enabled(bound) before making the draw call) */
		void Draw(int count) const;

		/* Draws the geometry indexed by ibo using triangles (The object to be drawn must be enabled(bound) before making the draw call) */
		void DrawIndexed(const class IndexBuffer& ibo) const;
	};
}