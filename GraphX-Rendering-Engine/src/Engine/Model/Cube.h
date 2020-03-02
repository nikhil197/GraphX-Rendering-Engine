#pragma once

#include "Mesh/Mesh3D.h"

#include "Engine/Core/Vertex.h"

namespace GraphX
{
	class Cube
		: public Mesh3D
	{
	public:
		/* Color is the base color of the cube used in the absence of textures */
		Cube(const GM::Vector3& Position, const GM::Vector3& Rotation, const GM::Vector3& Size, const Ref<Material>& Mat = nullptr);

		Cube(const Cube&) = delete;

		Cube(Cube&&) = delete;

		virtual void Enable() const override;

		virtual void Disable() const override;

		/* Returns the Vertex Positions of the cube */
		static const std::vector<GM::Vector3>& GetVertexPositions() { return s_VertexPositions; }

		/* Returns the vertices of the cube (including normals and texture coordinates) */
		static const std::vector<Vertex3D>& GetVertices() { return s_Vertices; }

		/* Returns the indices of the cube */
		static const std::vector<unsigned int>& GetIndices() { return s_Indices; }

	private:
		/* Vertex Positions of the Cube */
		static std::vector<GM::Vector3> s_VertexPositions;

		/* Vertices of the cube */
		static std::vector<Vertex3D> s_Vertices;

		/* Indices of the cube */
		static std::vector<unsigned int> s_Indices;
	};
}