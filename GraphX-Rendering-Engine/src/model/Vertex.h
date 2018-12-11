#pragma once

#include "GraphX_Vectors.h"

namespace engine
{
	/* A Structure to represent a vertex with all the attributes including color */
	struct VertexC
	{
		// The Position of the vertex
		gm::Vector3 Position;

		// The normal vector of the vertex
		gm::Vector3 Normal;
		
		// The color for the vertex
		gm::Vector4 Color;
	};

	/* A Structure to represent a vertex with all the attributes including texture coordinates */
	struct VertexT
	{
		// The Position of the vertex
		gm::Vector3 Position;

		// The normal vector of the vertex
		gm::Vector3 Normal;

		// The texture coordinate for the vertex
		gm::Vector2 TexCoord;
	};
}