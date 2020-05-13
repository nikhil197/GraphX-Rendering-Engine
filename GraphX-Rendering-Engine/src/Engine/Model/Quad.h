#pragma once

namespace GraphX
{
	class Quad
	{
	public:
		/* Number of vertices and indices for a quad */
		static constexpr uint32_t s_QuadVertexCount = 4;
		static constexpr uint32_t s_QuadIndicesCount = 6;

		/* Vertex positions and texture coords of a quad */
		static const GM::Vector3 s_QuadVertexPositions[4];
		static const GM::Vector2 s_QuadVertexTexCoords[4];

		/* Indices of a quad */
		static const uint32_t s_QuadIndices[6];
	public:
		Quad();

		/* Enables the quad for rendering */
		void Enable() const;

		/* Disables the quad after rendering */
		void Disable() const;

		~Quad();

	private:
		/* Vertex Array to store the vertices */
		Scope<class VertexArray> m_VAO;
	};
}