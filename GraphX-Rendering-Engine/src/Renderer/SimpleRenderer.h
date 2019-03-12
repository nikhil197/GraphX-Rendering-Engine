#pragma once

namespace engine
{
	class SimpleRenderer
	{
	public:
		/* Draws the Count number of indices (The object to be drawn must be enabled(bound) before making the draw call) */
		void Draw(int count) const;

		/* Draws the geometry indexed by ibo using triangles (The object to be drawn must be enabled(bound) before making the draw call) */
		void Draw(const class IndexBuffer& ibo) const;
	};
}