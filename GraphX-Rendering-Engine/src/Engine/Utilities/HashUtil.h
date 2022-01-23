#pragma once

#include "Textures/Texture2D.h"
#include "Vertex.h"

namespace GraphX
{
	/* Returns hash for the vertex positions */
	template<typename T>
	std::size_t GetVerticesHash(const std::vector<T>& Vertices)
	{
		std::size_t seed = 0;
		for (int i = 0; i < Vertices.size(); i++)
		{
			GM::Hash_Combine<GM::Vector3>(seed, Vertices[i].Position);
		}

		return seed;
	}

	/* Returns the hash value of a set of vertices and indices for instanced rendering */
	template<typename T>
	std::size_t	GetVerticesHash(const std::vector<T>& Vertices, const std::vector<uint32_t> Indices)
	{
		std::size_t seed = 0;
		for (int i = 0; i < Indices.size(); i++)
		{
			GM::Hash_Combine(seed, Vertices[Indices[i]].Position);
		}

		return seed;
	}

	std::size_t GetIndicesHash(const std::vector<uint32_t>& Indices);

	// Specializations
	template<>
	std::size_t GetVerticesHash<Vertex2D>(const std::vector<Vertex2D>& Vertices);

	template<>
	std::size_t GetVerticesHash<Vertex2DC>(const std::vector<Vertex2DC>& Vertices);

	template<>
	std::size_t GetVerticesHash<Vertex3D>(const std::vector<Vertex3D>& Vertices);

	template<>
	std::size_t GetVerticesHash<VertexBatch2D>(const std::vector<VertexBatch2D>& Vertices);

	template<>
	std::size_t GetVerticesHash<Vertex3DC>(const std::vector<Vertex3DC>& Vertices);
}