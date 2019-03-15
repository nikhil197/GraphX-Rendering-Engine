#pragma once

namespace engine
{
	class Quad
	{
	private:
		/* Vertices of the quad used to represent the particle */
		static std::vector<gm::Vector2> s_Vertices;
		
		/* Number of vertices in the quad */
		static const unsigned int s_QuadVertexCount;

		/* Vertex Array to store the vertices */
		class VertexArray* m_VAO;
	public:
		Quad();

		/* Enables the quad for rendering */
		void Enable() const;

		/* Disables the quad after rendering */
		void Disable() const;

		~Quad();

		/* Returns the vertices of the quad */
		static const std::vector<gm::Vector2>& GetVertices() { return s_Vertices; }

		/* Returns the vertices count for the quad */
		static const unsigned int GetVerticesCount() { return s_QuadVertexCount; }
	};
}