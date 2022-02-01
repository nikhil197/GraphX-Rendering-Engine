#pragma once

namespace GraphX
{
	class Texture2D;
	enum class FramebufferAttachmentType;
	class TextureLibrary
	{
	public:
		/* Adds a new texture to the library */
		void Add(const Ref<Texture2D>& texture);
		void Add(const std::string& name, const Ref<Texture2D>& texture);

		/* Createa a new texture2D, adds it to the library and returns a reference */
		Ref<Texture2D> LoadTexture2D(const std::string& FilePath, bool TileTexture = false, const std::string& name = "");
		Ref<Texture2D> LoadTexture2D(const std::string& name, uint32_t width, uint32_t height, FramebufferAttachmentType texType);
		Ref<Texture2D> LoadTexture2D(const std::string& name, uint32_t width, uint32_t height);

		/* Returns if the texture with a given name exists in the library or not */
		bool Exists(const std::string& name) const;

		/* Returns a reference to a texture2D already present in the library */
		Ref<Texture2D> GetTexture2D(const std::string& name);

	private:
		/* A map to store the reference to all the loaded textures */
		std::unordered_map<std::string, Ref<Texture2D>> m_TextureMap;
	};
}