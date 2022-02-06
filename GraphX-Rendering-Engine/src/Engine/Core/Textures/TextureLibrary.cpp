#include "pch.h"
#include "TextureLibrary.h"

#include "Texture2D.h"

namespace GraphX
{
	void TextureLibrary::Add(const Ref<Texture2D>& texture)
	{
		Add(texture->GetName(), texture);
	}

	void TextureLibrary::Add(const std::string& name, const Ref<Texture2D>& texture)
	{
		GX_ENGINE_ASSERT(!Exists(name), "Texture {0} already exists", name);
		m_TextureMap[name] = texture;
	}

	Ref<Texture2D> TextureLibrary::LoadTexture2D(const std::string& FilePath, bool TileTexture, const std::string& name)
	{
		Ref<Texture2D> texture = CreateRef<Texture2D>(FilePath, TileTexture, name);
		Add(texture->GetName(), texture);
		return texture;
	}

	Ref<Texture2D> TextureLibrary::LoadTexture2D(const std::string& name, uint32_t width, uint32_t height, FramebufferAttachmentType texType)
	{
		Ref<Texture2D> texture = CreateRef<Texture2D>(name, width, height, texType);
		Add(name, texture);
		return texture;
	}

	Ref<Texture2D> TextureLibrary::LoadTexture2D(const std::string& name, uint32_t width, uint32_t height)
	{
		Ref<Texture2D> texture = CreateRef<Texture2D>(name, width, height);
		Add(name, texture);
		return texture;
	}

	Ref<Texture2D> TextureLibrary::GetTexture2D(const std::string& name)
	{
		GX_ENGINE_ASSERT(Exists(name), "Texture {0} not found", name);
		return m_TextureMap[name];
	}

	bool TextureLibrary::Exists(const std::string& name) const
	{
		return m_TextureMap.find(name) != m_TextureMap.end();
	}
}