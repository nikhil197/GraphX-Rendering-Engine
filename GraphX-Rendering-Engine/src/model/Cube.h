#pragma once

#include "Mesh/Mesh3D.h"
#include "Mesh/Vertex.h"

namespace engine
{
	class Cube
		: Mesh3D
	{
	private:
		/* Vertices of the cube */
		static std::vector<Vertex3D> s_Vertices;

		/* Indices of the cube */
		static std::vector<unsigned int> s_Indices;

	public:
		/* Color is the base color of the cube used in the absence of textures */
		Cube(const gm::Vector3& Position, const gm::Vector3& Rotation, const gm::Vector3& Size, class Shader& Shader, const std::vector<const class Texture*>& Textures, const gm::Vector4& Color = gm::Vector4::ZeroVector);

		virtual void Enable() const override;

		virtual void Disable() const override;
	};
}