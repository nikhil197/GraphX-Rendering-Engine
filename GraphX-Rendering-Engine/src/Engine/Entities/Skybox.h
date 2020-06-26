#pragma once

#include "Entity.h"

namespace GraphX
{
	class CubeMap;

	class Skybox
		: public Entity
	{
	public:
		/* Speed at which the skybox will rotate */
		float RotationSpeed;

		/* Blend factor used to blend the cubemap and tint color */
		float BlendFactor;

	public:
		/*
		*	Creates a skybox
		* @param ShaderFilePath Path to the file containing the shader code for the skybox
		* @param FilePath Path to the folder containing the textures to be used to draw the skybox
		* @param FileNames names of the files to be used as textures for the six faces of the skybox. Order of the names should be right, left, top, bottom, front, back faces
		*/
		Skybox(const std::string& FilePath, const std::vector<std::string>& FileNames, const GM::Vector4& color, float factor = 0.0f, float Speed = 1.0f);

		void Update(float DeltaTime) override;

		/* Prepare the skybox to be rendered */
		void Enable() const;

		/* Unbind the skybox buffers and shader to */
		virtual void Disable() const override;

		/* Returns the skybox model matrix */
		inline const GM::Matrix4& GetModel() const { return m_Model; }

		/* Returns the color for tinting the cubemap */
		inline const GM::Vector4& GetTintColor() const { return m_Tint; }

		virtual ~Skybox();

	private:
		virtual void Enable(class Shader& shader, const std::string& Name = "") const override;

	private:
		/* Cubemap used to render the skybox */
		Ref<CubeMap> m_CubeMap;
		
		/* Current Rotation of the skybox */
		GM::Rotator m_Rotation;

		/* Model matrix required for the rotation of the skybox */
		GM::Matrix4 m_Model;

		/* Color to tint the cubemap with */
		GM::Vector4 m_Tint;
	};
}