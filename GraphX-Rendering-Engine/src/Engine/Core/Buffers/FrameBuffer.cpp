#include "pch.h"
#include "FrameBuffer.h"
#include "GL/glew.h"

#include "Textures/Texture2D.h"

namespace GraphX
{
	FrameBuffer::FrameBuffer(int width, int height, FramebufferType Type, const std::string& name)
		: RendererResource(), m_Width(width), m_Height(height)
	{
		GX_PROFILE_FUNCTION()

		glGenFramebuffers(1, &m_RendererID);
		if (Type == FramebufferType::GX_FRAME_DEPTH)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
			m_DepthMap = CreateRef<Texture2D>(name, m_Width, m_Height, FramebufferAttachmentType::GX_TEX_DEPTH);
			m_DepthMap->Bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap->m_RendererID, 0);
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}
		else
			m_DepthMap = nullptr;

		GX_ENGINE_ASSERT(m_DepthMap != nullptr && glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Unable to Create Framebuffer");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::InitRHI()
	{

	}

	void FrameBuffer::ReleaseRHI()
	{

	}

	void FrameBuffer::Bind() const
	{
		GX_PROFILE_FUNCTION()

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}

	void FrameBuffer::UnBind() const
	{
		GX_PROFILE_FUNCTION()

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::BindDepthMap(unsigned int slot) const
	{
		GX_PROFILE_FUNCTION()

		if(m_DepthMap)
			m_DepthMap->Bind(slot);
	}

	void FrameBuffer::UnBindDepthMap() const
	{
		GX_PROFILE_FUNCTION()

		if(m_DepthMap)
			m_DepthMap->UnBind();
	}

	FrameBuffer::~FrameBuffer()
	{
		GX_PROFILE_FUNCTION()

		glDeleteFramebuffers(1, &m_RendererID);
	}
}