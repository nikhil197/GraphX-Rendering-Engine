#pragma once

#include "GraphX_Vectors.h"
#include "buffers/VertexBufferLayout.h"

namespace engine
{
	struct Vertex
	{
		static VertexBufferLayout GetVertexLayout()
		{
			return VertexBufferLayout();
		}
	};

	/* A Structure to represent a 2D vertex with all the attributes including color */
	struct Vertex2DC : public Vertex
	{
		// The Position of the vertex
		gm::Vector2 Position;

		// The color for the vertex
		gm::Vector4 Color;

		Vertex2DC(const gm::Vector2& position, const gm::Vector4& color)
			: Position(position), Color(color)
		{}

		// Returns the vertexbufferlayout for this struct
		static VertexBufferLayout GetVertexLayout()
		{
			VertexBufferLayout layout;
			layout.Push<float>(gm::Vector2::Components);	// For positions
			layout.Push<float>(gm::Vector4::Components);	// For colors

			return layout;
		}
	};

	/* A Structure to represent a vertex 2D with all the attributes including texture coordinates */
	struct Vertex2DT : public Vertex
	{
		// The Position of the vertex
		gm::Vector2 Position;

		// The texture coordinate for the vertex
		gm::Vector2 TexCoord;

		Vertex2DT(const gm::Vector2& position, const gm::Vector2& texCoord)
			: Position(position), TexCoord(texCoord)
		{}

		// Returns the vertexbufferlayout for this struct
		static VertexBufferLayout GetVertexLayout()
		{
			VertexBufferLayout layout;
			layout.Push<float>(gm::Vector2::Components);	// For positions
			layout.Push<float>(gm::Vector2::Components);	// For texture coordinates

			return layout;
		}
	};

	/* A Structure to represent a vertex with all the attributes including color */
	struct VertexC : public Vertex
	{
		// The Position of the vertex
		gm::Vector3 Position;

		// The normal vector of the vertex
		gm::Vector3 Normal;
		
		// The color for the vertex
		gm::Vector4 Color;

		VertexC(const gm::Vector3& pos, const gm::Vector3& normal, const gm::Vector4& color)
			: Position(pos), Normal(normal), Color(color)
		{ }

		// Returns the vertexbufferlayout for this struct
		static VertexBufferLayout GetVertexLayout()
		{
			VertexBufferLayout layout;
			layout.Push<float>(gm::Vector3::Components);	// For positions
			layout.Push<float>(gm::Vector3::Components);	// For Normals
			layout.Push<float>(gm::Vector4::Components);	// For colors

			return layout;
		}
	};

	/* A Structure to represent a vertex with all the attributes including texture coordinates */
	struct VertexT : public Vertex
	{
		// The Position of the vertex
		gm::Vector3 Position;

		// The normal vector of the vertex
		gm::Vector3 Normal;

		// The texture coordinate for the vertex
		gm::Vector2 TexCoord;

		VertexT(const gm::Vector3& pos, const gm::Vector3& normal, const gm::Vector2& texCoord)
			: Position(pos), Normal(normal), TexCoord(texCoord)
		{ }

		// Returns the vertexbufferlayout for this struct
		static VertexBufferLayout GetVertexLayout()
		{
			VertexBufferLayout layout;
			layout.Push<float>(gm::Vector3::Components);	// For positions
			layout.Push<float>(gm::Vector3::Components);	// For Normals
			layout.Push<float>(gm::Vector2::Components);	// For texture coordinates

			return layout;
		}
	};
}