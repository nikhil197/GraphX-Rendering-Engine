#include "pch.h"

#include "Cube.h"
#include "Shaders/Shader.h"

namespace engine
{
	using namespace gm;

	/* Initialise the vertices */
	std::vector<Vertex3D> Cube::s_Vertices = {
			/*Vertex Positions*/			/* Normal Coordinates */		/* Texture Coordinates */
		// Front face
		{ Vector3(-1.0f, -1.0f,  1.0f),	  Vector3(-1.0f, -1.0f,  1.0f),		Vector2(0.0f, 0.0f) },	//0
		{ Vector3( 1.0f, -1.0f,  1.0f),	  Vector3( 1.0f, -1.0f,  1.0f),		Vector2(1.0f, 0.0f) },	//1
		{ Vector3( 1.0f,  1.0f,  1.0f),	  Vector3( 1.0f,  1.0f,  1.0f),		Vector2(1.0f, 1.0f) },	//2
		{ Vector3(-1.0f,  1.0f,  1.0f),	  Vector3(-1.0f,  1.0f,  1.0f),		Vector2(0.0f, 1.0f) },	//3

		// Back face			
		{ Vector3(-1.0f, -1.0f, -1.0f),	  Vector3(-1.0f, -1.0f, -1.0f),		Vector2(1.0f, 0.0f) },	//4
		{ Vector3( 1.0f, -1.0f, -1.0f),	  Vector3( 1.0f, -1.0f, -1.0f),		Vector2(0.0f, 0.0f) },	//5
		{ Vector3( 1.0f,  1.0f, -1.0f),	  Vector3( 1.0f,  1.0f, -1.0f),		Vector2(0.0f, 1.0f) },	//6
		{ Vector3(-1.0f,  1.0f, -1.0f),	  Vector3(-1.0f,  1.0f, -1.0f),		Vector2(1.0f, 1.0f) },	//7

		// Top face (Needed because of the different texture coordinates)
		{ Vector3(-1.0f,  1.0f,  1.0f),	  Vector3(-1.0f,  1.0f,  1.0f),		Vector2(0.0f, 0.0f) },	//8
		{ Vector3( 1.0f,  1.0f,  1.0f),	  Vector3( 1.0f,  1.0f,  1.0f),		Vector2(1.0f, 0.0f) },	//9
		{ Vector3( 1.0f,  1.0f, -1.0f),	  Vector3( 1.0f,  1.0f, -1.0f),		Vector2(1.0f, 1.0f) },	//10
		{ Vector3(-1.0f,  1.0f, -1.0f),	  Vector3(-1.0f,  1.0f, -1.0f),		Vector2(0.0f, 1.0f) },	//11

		// Bottom Face (Needed because of the different texture coordinates)
		{ Vector3(-1.0f, -1.0f,  1.0f),	  Vector3(-1.0f, -1.0f,  1.0f),		Vector2(0.0f, 1.0f) },	//12
		{ Vector3( 1.0f, -1.0f,  1.0f),	  Vector3( 1.0f, -1.0f,  1.0f),		Vector2(1.0f, 1.0f) },	//13
		{ Vector3( 1.0f, -1.0f, -1.0f),	  Vector3( 1.0f, -1.0f, -1.0f),		Vector2(1.0f, 0.0f) },	//14
		{ Vector3(-1.0f, -1.0f, -1.0f),	  Vector3(-1.0f, -1.0f, -1.0f),		Vector2(0.0f, 0.0f) }	//15

		// If the above vertices for top and bottom face are not provided, the texture will not be drawn on these surface
		// The texture will be flipped horizontally for the adjacent faces of the cube (Front, left, right, back)
		// If this effect is to be removed, separate vertices for all the faces will be required since they all use different texture coordinates
	};

	/* Initialise the indices */
	std::vector<unsigned int> Cube::s_Indices = {
		// Front face
		3, 0, 2,
		2, 0, 1,

		// Top Face
		11, 8, 10,
		10, 8, 9,

		// Back Face
		6, 5, 7,
		7, 5, 4,

		// Bottom Face
		12, 15, 13,
		13, 15, 14,

		// Right face
		2, 1, 6,
		6, 1, 5,

		// Left Face
		7, 4, 3,
		3, 4, 0
	};

	Cube::Cube(const gm::Vector3& Position, const gm::Vector3& Rotation, const gm::Vector3& Size, class Shader& Shader, const std::vector<const Texture*>& Textures, const gm::Vector4& Color)
		: Mesh3D(Position, Rotation, Size, Shader, Textures, s_Vertices, s_Indices, Color)
	{
	}

	void Cube::Enable() const
	{
		Mesh3D::Enable();
	}

	void Cube::Disable() const
	{
		Mesh3D::Disable();
	}
}