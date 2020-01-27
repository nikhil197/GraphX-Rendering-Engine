#pragma once

namespace GraphX
{
	using namespace GM;

	class Texture;
	class Shader;

	class Material
	{
	public:
		Material(Shader* shader);

		/* Copy Constructor (For now, This doesn't copy the textues, For that Use Material Instance once created) */
		Material(const Material& Other);

		void Bind();

		/* Add another texture to the material */
		void AddTexture(const Texture* Tex);
		void AddTexture(const std::vector<const Texture*>& Textures);

		/* Sets a new value for Base Color */
		inline void SetBaseColor(const Vector4& NewColor)
		{
			m_BaseColor = NewColor;
		}

		/* Sets a new value for Specular Strengt (Reflectivity) */
		inline void SetSpecularStrength(const float NewStrength)
		{
			m_Specular = NewStrength;
		}
		
		/* Sets a new value for shininess */
		inline void SetShininess(const float Shininess)
		{
			m_Shininess = Shininess;
		}

		/* Returns Base color of the material */
		inline const Vector4& GetBaseColor() const { return m_BaseColor; }

		/* Returns Specular Strength of the material */
		inline float GetSpecularStrength() const { return m_Specular; }

		/* Returns Shininess of the material */
		inline float GetShininess() const { return m_Shininess; }

		/* Returns the shader of the material */
		inline Shader* GetShader() const { return m_Shader; }

		/* Returns textures used in the material */
		inline const std::vector<const Texture*>& GetTextures() const { return m_Textures; }

		~Material() = default;

	protected:
		/* Base Material Color */
		Vector4 m_BaseColor;

		/* Specular Strength */
		float m_Specular = 0.5f;

		/* Shininess (how well object reflects light) */
		float m_Shininess = 32.0f;
		
		/* Shader */
		Shader* m_Shader;

		/* Textures for the material */
		std::vector<const Texture*> m_Textures;
	};
}