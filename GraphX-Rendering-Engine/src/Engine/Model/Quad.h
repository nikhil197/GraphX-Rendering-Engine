#pragma once

namespace GraphX
{
	class Quad
	{
	public:
		Quad();

		/* Enables the quad for rendering */
		void Enable() const;

		/* Disables the quad after rendering */
		void Disable() const;

		~Quad();

		/* Returns the vertices of the quad */
		static const std::vector<GM::Vector2>& GetVertices() { return s_Vertices; }

		/* Returns the vertices count for the quad */
		static const unsigned int GetVerticesCount() { return s_QuadVertexCount; }

	private:
		/* Vertices of the quad used to represent the particle */
		static std::vector<GM::Vector2> s_Vertices;

		/* Number of vertices in the quad */
		static const unsigned int s_QuadVertexCount;

		/* Vertex Array to store the vertices */
		Scope<class VertexArray> m_VAO;
	};
}