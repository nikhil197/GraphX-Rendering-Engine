#pragma once

namespace engine
{
	/* What frame buffer will be used for */
	enum FramebufferType
	{
		GX_FRAME_DEPTH		/* Framebuffer used for depth buffer */
	};

	class FrameBuffer
	{
	private:
		/* ID used by the renderer for this buffer */
		unsigned int m_RendererID;

		/* Width and height of the framebuffer (equal to the screen size) */
		int m_Width, m_Height;

		/* Depth Texture attachment for the frame buffer */
		class Texture* m_DepthMap;

	public:
		/* Constructor */
		/* @param width - width of the framebuffer */
		/* @param height - height of the framebuffer */
		/* @param IsScreenSize - whether the framebuffer size is equal to that of the screen (main window) */
		FrameBuffer(int width, int height, FramebufferType Type);

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

		~FrameBuffer();
	};
}