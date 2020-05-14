#include "framebuffer.h"

namespace audioengineer {
	namespace graphics {

		FrameBuffer::FrameBuffer(uint width, uint height, AttachmentType colorType, AttachmentType depthType, AttachmentType stencilType)
		{
			m_width = width;
			m_height = height;
			m_screenW = 0;
			m_screenH = 0;

			glGenFramebuffers(1, &m_ID);
			m_colorID = 0;
			m_depthID = 0;
			m_stencilID = 0;
			m_colorType = NONE;
			m_depthType = NONE;
			m_stencilType = NONE;
			glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
			setColorAttachment(colorType);
			setDepthAttachment(depthType);
			setStencilAttachment(stencilType);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		FrameBuffer::~FrameBuffer()
		{
			glDeleteFramebuffers(1, &m_ID);
			setColorAttachment(NONE);
			setDepthAttachment(NONE);
			setStencilAttachment(NONE);
		}

		GLuint FrameBuffer::setColorAttachment(AttachmentType colorType)
		{
			if (m_colorType == TEXTURE) {
				glDeleteTextures(1, &m_colorID);
			}
			else if (m_colorType == RENDERBUFFER) {
				glDeleteRenderbuffers(1, &m_colorID);
			}

			m_colorType = colorType;
			if (colorType == TEXTURE) {
				glGenTextures(1, &m_colorID);
				glBindTexture(GL_TEXTURE_2D, m_colorID);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_colorID, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else if (colorType == RENDERBUFFER) {
				glGenRenderbuffers(1, &m_colorID);
				glBindRenderbuffer(GL_RENDERBUFFER, m_colorID);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_COLOR_ATTACHMENT0, m_width, m_height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorID);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
			}
			else {
				m_colorID = 0;
			}
			return m_colorID;
		}

		GLuint FrameBuffer::setDepthAttachment(AttachmentType depthType)
		{
			if (m_depthType == TEXTURE)
				glDeleteTextures(1, &m_depthID);
			else if (m_depthType == RENDERBUFFER)
				glDeleteRenderbuffers(1, &m_depthID);

			m_depthType = depthType;
			if (depthType == TEXTURE) {
				glGenTextures(1, &m_depthID);
				glBindTexture(GL_TEXTURE_2D, m_depthID);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glFramebufferTexture(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, m_depthID, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else if (depthType == RENDERBUFFER) {
				glGenRenderbuffers(1, &m_depthID);
				glBindRenderbuffer(GL_RENDERBUFFER, m_depthID);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthID);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
			}
			else {
				m_depthID = 0;
			}
			return m_depthID;
		}

		GLuint FrameBuffer::setStencilAttachment(AttachmentType stencilType)
		{
			if (m_stencilType == TEXTURE)
				glDeleteTextures(1, &m_stencilID);
			else if (m_stencilType == RENDERBUFFER)
				glDeleteRenderbuffers(1, &m_stencilID);

			m_stencilType = stencilType;
			if (stencilType == TEXTURE) {
				glGenTextures(1, &m_stencilID);
				glBindTexture(GL_TEXTURE_2D, m_stencilID);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glFramebufferTexture(GL_RENDERBUFFER, GL_STENCIL_ATTACHMENT, m_stencilID, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			else if (stencilType == RENDERBUFFER) {
				glGenRenderbuffers(1, &m_stencilID);
				glBindRenderbuffer(GL_RENDERBUFFER, m_stencilID);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_ATTACHMENT, m_width, m_height);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_stencilID);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
			}
			else {
				m_stencilID = 0;
			}
			return m_stencilID;
		}

		void FrameBuffer::bind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
			glViewport(0, 0, m_width, m_height);
		}

		void FrameBuffer::unbind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, m_screenW, m_screenH);
		}

		Texture FrameBuffer::getColorTexture() const
		{
			if (m_colorType != TEXTURE) return nullptr;
			return Texture(m_colorID, m_width, m_height);
		}

		Texture FrameBuffer::getDepthTexture() const
		{
			if (m_depthType != TEXTURE) return nullptr;
			return Texture(m_depthID, m_width, m_height);
		}

		Texture FrameBuffer::getStencilTexture() const
		{
			if (m_stencilType != TEXTURE) return nullptr;
			return Texture(m_stencilID, m_width, m_height);
		}

		void FrameBuffer::setSize(uint width, uint height)
		{
			m_width = width;
			m_height = height;
			glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
			setColorAttachment(m_colorType);
			setDepthAttachment(m_depthType);
			setStencilAttachment(m_stencilType);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void FrameBuffer::setScreenSize(uint screenW, uint screenH)
		{
			m_screenW = screenW;
			m_screenH = screenH;
		}
	}
}