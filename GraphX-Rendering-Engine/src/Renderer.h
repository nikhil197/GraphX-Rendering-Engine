#pragma once

namespace engine
{
	class VertexArray;
	class IndexBuffer;
	class Shader;

	class Renderer
	{
	public:
		/* To draw the primitives on the screen */
		void Draw(const VertexArray& vao, const Shader& shader, int count) const;

		/* To draw the primitives on the screen using indices */
		void Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const;
	};
}