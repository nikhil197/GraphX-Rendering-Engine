#pragma once

#include "Engine/Core/RendererResource.h"

namespace GraphX
{
	/* What frame buffer will be used for */
	enum class FramebufferType
	{
		GX_FRAME_DEPTH		/* Framebuffer used for depth buffer */
	};

	class FrameBuffer
		: public RendererResource
	{
	public:
		/* Constructor */
		/* @param width - width of the framebuffer */
		/* @param height - height of the framebuffer */
		/* @param Type - Type of the Framebuffer */
		/* @param name - Name of the buffer */
		FrameBuffer(int width, int height, FramebufferType Type, const std::string& name);

		/* Binds the frame buffer */
		void Bind() const;

		/* Un binds the framebuffer */
		void UnBind() const;

		/* Binds the depth map of the frame buffer */
		void BindDepthMap(unsigned int slot = 0) const;

		/* Un binds the depth map of the frame buffer */
		void UnBindDepthMap() const;

		/* Returns the width of the buffer used for the framebuffer */
		inline int GetWidth() const { return m_Width; }

		/* Returns the height of the buffer used for the framebuffer */
		inline int GetHeight() const { return m_Height; }

		/* Returns the depth map for this framebuffer */
		inline Ref<class Texture2D> GetDepthMap() const { return m_DepthMap; }

		~FrameBuffer();

		/* Renderer Resource Interface */
		virtual void InitRHI() override;

		virtual void ReleaseRHI() override;

		/** ~ Renderer Resource interface */

	private:
		/* Width and height of the framebuffer (equal to the screen size) */
		int m_Width, m_Height;

		/* Depth Texture attachment for the frame buffer */
		Ref<class Texture2D> m_DepthMap;
	};
}