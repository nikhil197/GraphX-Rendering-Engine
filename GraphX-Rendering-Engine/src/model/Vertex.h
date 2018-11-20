#pragma once

#include "GraphX_Vectors.h"

namespace engine
{
	/* A Structure to represent a vertex with all the attributes */
	struct Vertex
	{
		// The Position of the vertex
		gm::Vector3 Position;

		// The normal vector of the vertex
		gm::Vector3 Normal;
		
		// The color for the vertex
		gm::Vector4 Color;
	};
}