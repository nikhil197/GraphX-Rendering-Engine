#include "pch.h"
#include "HashUtil.h"

#include "Textures/Texture2D.h"
#include "Materials/Material.h"

template<>
struct std::hash<GraphX::Texture2D>
{
	std::size_t operator()(GraphX::Texture2D const& Tex) const noexcept
	{
		std::size_t seed = 0;
		GM::Hash_Combine(seed, Tex.GetName());
		GM::Hash_Combine(seed, Tex.GetWidth());
		GM::Hash_Combine(seed, Tex.GetHeight());

		return seed;
	}
};

template<>
struct std::hash<GraphX::Material>
{
	std::size_t operator()(GraphX::Material const& Mat) const noexcept
	{
		std::size_t seed = 0;
		
		// Hash the Base Color
		GM::Hash_Combine(seed, Mat.GetBaseColor());

		// Hash the textures
		for (const auto& tex : Mat.GetTextures())
		{
			GM::Hash_Combine(seed, tex);
		}

		return seed;
	}
};

namespace GraphX
{
	// Definition of specializations
	template<>
	std::size_t GetVerticesHash<Vertex2D>(const std::vector<Vertex2D>& Vertices)
	{
		std::size_t seed = 0;
		for (int i = 0; i < Vertices.size(); i++)
		{
			GM::Hash_Combine(seed, Vertices[i].Position);
			GM::Hash_Combine(seed, Vertices[i].TexCoord);
		}

		return seed;
	}

	template<>
	std::size_t GetVerticesHash<Vertex2DC>(const std::vector<Vertex2DC>& Vertices)
	{
		std::size_t seed = 0;
		for (int i = 0; i < Vertices.size(); i++)
		{
			GM::Hash_Combine(seed, Vertices[i].Position);
			GM::Hash_Combine(seed, Vertices[i].Color);
		}

		return seed;
	}

	template<>
	std::size_t GetVerticesHash<Vertex3D>(const std::vector<Vertex3D>& Vertices)
	{
		std::size_t seed = 0;
		for (int i = 0; i < Vertices.size(); i++)
		{
			GM::Hash_Combine(seed, Vertices[i].Position);
			GM::Hash_Combine(seed, Vertices[i].Normal);
			GM::Hash_Combine(seed, Vertices[i].TexCoord);
		}

		return seed;
	}

	template<>
	std::size_t GetVerticesHash<VertexBatch2D>(const std::vector<VertexBatch2D>& Vertices)
	{
		std::size_t seed = 0;
		for (int i = 0; i < Vertices.size(); i++)
		{
			GM::Hash_Combine(seed, Vertices[i].Position);
			GM::Hash_Combine(seed, Vertices[i].Color);
			GM::Hash_Combine(seed, Vertices[i].TexCoords);
			GM::Hash_Combine(seed, Vertices[i].TexIndex);
		}

		return seed;
	}

	template<>
	std::size_t GetVerticesHash<Vertex3DC>(const std::vector<Vertex3DC>& Vertices)
	{
		std::size_t seed = 0;
		for (int i = 0; i < Vertices.size(); i++)
		{
			GM::Hash_Combine(seed, Vertices[i].Position);
			GM::Hash_Combine(seed, Vertices[i].Normal);
			GM::Hash_Combine(seed, Vertices[i].Color);
		}

		return seed;
	}

	std::size_t GetIndicesHash(const std::vector<uint32_t>& Indices)
	{
		size_t seed = 0;
		for (int i = 0; i < Indices.size(); i++)
		{
			GM::Hash_Combine(seed, Indices[i]);
		}
		return seed;
	}
}