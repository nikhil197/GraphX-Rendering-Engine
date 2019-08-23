#pragma once

#include "GraphX_Vectors.h"
#include "Buffers/VertexBufferLayout.h"

namespace GraphX
{
	/* A Structure to represent a 2D vertex with all the attributes including color (Keeping this for the testing purposes */
	struct Vertex2DC
	{
		// The Position of the vertex
		GraphXMaths::Vector2 Position;

		// The color for the vertex
		GraphXMaths::Vector4 Color;

		// VertexBufferLayout for this struct
		static const VertexBufferLayout& VertexLayout()
		{
			static VertexBufferLayout Layout = {
				{ BufferDataType::Float2 },		// For positions
				{ BufferDataType::Float4 }		// For Colors
			};

			return Layout;
		}
	};

	/* A Structure to represent a vertex 2D with all the attributes including texture coordinates */
	struct Vertex2D
	{
		// The Position of the vertex
		GraphXMaths::Vector2 Position;

		// The texture coordinate for the vertex
		GraphXMaths::Vector2 TexCoord;

		// VertexBufferLayout for this struct
		static const VertexBufferLayout& VertexLayout()
		{
			static VertexBufferLayout Layout = {
				{ BufferDataType::Float2 },		// For positions
				{ BufferDataType::Float2 }		// For texture coordinates
			};

			return Layout;
		}
	};

	/* A Structure to represent a vertex with all the attributes including color (Only keeping this for the testing purposes) */
	struct Vertex3DC
	{
		// The Position of the vertex
		GraphXMaths::Vector3 Position;

		// The normal vector of the vertex
		GraphXMaths::Vector3 Normal;
		
		// The color for the vertex
		GraphXMaths::Vector4 Color;

		// VertexBufferLayout for this struct
		static const VertexBufferLayout& VertexLayout()
		{
			static VertexBufferLayout Layout = {
				{ BufferDataType::Float3 },	// For positions
				{ BufferDataType::Float3 },	// For Normals
				{ BufferDataType::Float4 }	// For Colors
			};

			return Layout;
		}
	};

	/* A Structure to represent a vertex with all the attributes including texture coordinates */
	struct Vertex3D
	{
		// The Position of the vertex
		GraphXMaths::Vector3 Position;

		// The normal vector of the vertex
		GraphXMaths::Vector3 Normal;

		// The texture coordinate for the vertex
		GraphXMaths::Vector2 TexCoord;

		// VertexBufferLayout for this struct
		static const VertexBufferLayout& VertexLayout()
		{
			static VertexBufferLayout Layout = {
				{ BufferDataType::Float3 },	// For positions
				{ BufferDataType::Float3 },	// For Normals
				{ BufferDataType::Float2 }	// For texture coordinates
			};

			return Layout;
		}
	};
}