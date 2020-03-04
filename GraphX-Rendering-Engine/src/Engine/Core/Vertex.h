#pragma once

#include "GraphX_Vectors.h"
#include "Buffers/VertexBufferLayout.h"

namespace GraphX
{
	/* A Structure to represent a 2D vertex with all the attributes including color (Keeping this for the testing purposes */
	struct Vertex2DC
	{
		// The Position of the vertex
		GM::Vector2 Position;

		// The color for the vertex
		GM::Vector4 Color;

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
		GM::Vector3 Position;

		// The texture coordinate for the vertex
		GM::Vector2 TexCoord;

		// VertexBufferLayout for this struct
		static const VertexBufferLayout& VertexLayout()
		{
			static VertexBufferLayout Layout = {
				{ BufferDataType::Float3 },		// For positions
				{ BufferDataType::Float2 }		// For texture coordinates
			};

			return Layout;
		}
	};

	/* A Structure to represent a vertex with all the attributes including color (Only keeping this for the testing purposes) */
	struct Vertex3DC
	{
		// The Position of the vertex
		GM::Vector3 Position;

		// The normal vector of the vertex
		GM::Vector3 Normal;
		
		// The color for the vertex
		GM::Vector4 Color;

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
		GM::Vector3 Position;

		// The normal vector of the vertex
		GM::Vector3 Normal;

		// The texture coordinate for the vertex
		GM::Vector2 TexCoord;

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

	/* Structure to represent a vertex of 'Batch2D' */
	struct VertexBatch2D
	{
		// Position of the vertex
		GM::Vector3 Position;

		// Color of the vertex
		GM::Vector4 Color;
		
		// Texture coordinates of the vertex
		GM::Vector2 TexCoords;
		
		// Texture slot used for this vertex
		float TexIndex;

		static const VertexBufferLayout& VertexLayout()
		{
			static VertexBufferLayout Layout = {
				{ BufferDataType::Float3 },	// For Position
				{ BufferDataType::Float4 },	// For Color
				{ BufferDataType::Float2 },	// For TexCoords
				{ BufferDataType::Float }	// For TexIndex 
			};

			return Layout;
		}
	};

	/* Structure to represent a vertex of 'ParticleBatch' */
	struct VertexParticleBatch
	{
		// Position of the vertex
		GM::Vector3 Position;

		// Color of the vertex
		GM::Vector4 Color;

		// Texture coordinates of the vertex
		GM::Vector2 TexCoords;

		// Texture offsets for the particle(In case the particle uses a texture atlas)
		GM::Vector4 TexOffsets;

		// No. of rows in the texture atlas
		float TexAtlasRows;

		// Blend factor for blending b/w two stages of the atlas
		float BlendFactor;

		// Texture slot used for this vertex
		float TexIndex;

		static const VertexBufferLayout& VertexLayout()
		{
			static VertexBufferLayout Layout = {
				{ BufferDataType::Float3 },	// For Position
				{ BufferDataType::Float4 },	// For Color
				{ BufferDataType::Float2 },	// For TexCoords
				{ BufferDataType::Float4 },	// For TexOffsets 
				{ BufferDataType::Float },	// For TexAtlasRows 
				{ BufferDataType::Float },	// For BlendFactor 
				{ BufferDataType::Float },	// For TexIndex
			};

			return Layout;
		}
	};
}