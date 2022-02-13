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
		float TexIndex = 0.0f;

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
		GM::Vector2 TexCoords1;

		// Texture offsets for the particle(In case the particle uses a texture atlas)
		GM::Vector2 TexCoords2;

		// No. of rows in the texture atlas
		float TexAtlasRows = 1.0f;

		// Blend factor for blending b/w two stages of the atlas
		float BlendFactor = 0.0f;

		// Texture slot used for this vertex
		float TexIndex = 0.0f;

		static const VertexBufferLayout& VertexLayout()
		{
			static VertexBufferLayout Layout = {
				{ BufferDataType::Float3 },	// For Position
				{ BufferDataType::Float4 },	// For Color
				{ BufferDataType::Float2 },	// For TexCoords1
				{ BufferDataType::Float2 },	// For TexCoords2 
				{ BufferDataType::Float },	// For TexAtlasRows 
				{ BufferDataType::Float },	// For BlendFactor 
				{ BufferDataType::Float },	// For TexIndex
			};

			return Layout;
		}
	};

	/* Data that varies for each Instance */
	struct PerInstanceData
	{
		// Model Matrix
		GM::Matrix4 ModelMatrix;

		// Normal Matrix;
		GM::Matrix3 NormalMatrix;

		// The tint color for the instance
		GM::Vector4 TintColor;

		// The reflectivity of the instance
		float Reflectivity;

		// The Shininess of the instance surface
		float Shininess;

		static const VertexBufferLayout& VertexLayout()
		{
			static VertexBufferLayout Layout = {
				{ BufferDataType::Mat4, false, 1 },		// For Model Matrix
				{ BufferDataType::Mat3, false, 1 },		// For Normal Matrix
				{ BufferDataType::Float4, false, 1 },	// For Tint Color
				{ BufferDataType::Float, false, 1 },	// For Reflectivity 
				{ BufferDataType::Float, false, 1 },	// For Shininess
			};

			return Layout;
		}
	};
}