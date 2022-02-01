#pragma once

#include "Engine/Core/RendererResource.h"

namespace GraphX
{
	/* Type of the texture used for the frame buffer */
	enum class FramebufferAttachmentType
	{
		GX_TEX_NONE	,		/* Normal Texture */
		GX_TEX_COLOR,		/* Texture is used as a color buffer for a framebuffer */
		GX_TEX_DEPTH		/* Texture is used as a depth buffer for a framebuffer */
	};

	class Texture2D
		: public RendererResource
	{
		/* Required to access the rendererID for the texture to bind to the framebuffer */
		friend class FrameBuffer;

	public:
		/* Constructor */
		/* @Param TileTexture - Whether the texture will be used for tilling or not */
		Texture2D(const std::string& filePath, bool TileTexture = false, const std::string& Name = "");

		/* Constructor for the framebuffer textures */
		Texture2D(const std::string& name, uint32_t width, uint32_t height, FramebufferAttachmentType texType);

		/* Constructor for creating texture by code (Data needs to be set manually) */
		Texture2D(const std::string& name, uint32_t width, uint32_t height);

		/* Bind the texture to the indicated slot */
		void Bind(unsigned int slot = 0) const;

		/* Unbind the currently bound texture */
		void UnBind() const;

		/* Renderer Resource Interface */
		virtual void InitRHI() override;

		virtual void ReleaseRHI() override;

		/** ~ Renderer Resource interface */

		// Set Data of the texture
		void SetData(void* data, uint32_t size);

		/* Returns the width of the texture */
		inline uint32_t GetWidth() const { return m_Width; }

		/* Returns the height of the texture */
		inline uint32_t GetHeight() const { return m_Height; }

		/* Returns the name of the texture */
		inline const std::string& GetName() const { return m_Name; }

		/* Returns whether the texture will be used for tiling */
		inline bool IsTileTexture() const { return m_TileTexture; }

		/* Returns the number of rows in the texture atlas */
		virtual uint32_t GetRowsInAtlas() const { return 1; }

		/* Whether this is a texture atlas (or sprite sheet)*/
		virtual bool IsSpriteSheet() const { return false; }

		/* Equality Test for the texture */
		bool operator==(const Texture2D& OtherTex) const;

		/* Destroy the texture */
		virtual ~Texture2D();

	protected:
		/* Path to the texture file */
		std::string m_Name;

		/* Width and height of the texture */
		uint32_t m_Width, m_Height;

		/* Format of the texture storage and the supplied data */
		uint32_t m_InternalFormat, m_DataFormat;

		/* Whether the texture will be used for tiling or not */
		bool m_TileTexture;
	};

	/* Equality test for a Texture wrapped in reference_wrapper */
	bool operator==(const std::reference_wrapper<Texture2D>& Ref1, const std::reference_wrapper<Texture2D>& Ref2);
}