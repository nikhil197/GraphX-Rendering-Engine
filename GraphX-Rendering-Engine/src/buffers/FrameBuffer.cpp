#include "pch.h"
#include "FrameBuffer.h"

#include "Textures/Texture.h"

namespace engine
{
	FrameBuffer::FrameBuffer(int width, int height, FramebufferType Type)
		: m_RendererID(0), m_Width(width), m_Height(height)
	{
		GLCall(glGenFramebuffers(1, &m_RendererID));
		if (Type == FramebufferType::GX_FRAME_DEPTH)
		{
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
			m_DepthMap = new Texture(m_Width, m_Height, FramebufferAttachmentType::GX_TEX_DEPTH);
			m_DepthMap->Bind();
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap->m_RendererID, 0));
			GLCall(glDrawBuffer(GL_NONE));
			GLCall(glReadBuffer(GL_NONE));
		}
		else
			m_DepthMap = nullptr;

		ASSERT(m_DepthMap != nullptr && glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		UnBind();
	}

	void FrameBuffer::Bind() const
	{	
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID));
	}

	void FrameBuffer::UnBind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void FrameBuffer::BindDepthMap(unsigned int slot) const
	{
		if(m_DepthMap)
			m_DepthMap->Bind(slot);
	}

	void FrameBuffer::UnBindDepthMap() const
	{
		if(m_DepthMap)
			m_DepthMap->UnBind();
	}

	FrameBuffer::~FrameBuffer()
	{
		GLCall(glDeleteFramebuffers(1, &m_RendererID));
	}
}