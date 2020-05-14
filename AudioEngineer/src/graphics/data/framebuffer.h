#pragma once

#include <vector>
#include <GL\glew.h>
#include "..\renderables\texture.h"
#include "..\..\events\eventsystem.h"
#include "..\..\defines.h"

namespace audioengineer {
	namespace graphics {

		enum AttachmentType
		{
			TEXTURE,
			RENDERBUFFER,
			NONE
		};

		class FrameBuffer
		{
		private:
			GLuint m_ID;
			GLuint m_colorID;
			GLuint m_depthID;
			GLuint m_stencilID;
			AttachmentType m_colorType;
			AttachmentType m_depthType;
			AttachmentType m_stencilType;
			uint m_width;
			uint m_height;
			uint m_screenW;
			uint m_screenH;
			
		public:
			FrameBuffer(uint width, uint height, AttachmentType colorType, AttachmentType depthType = NONE, AttachmentType stencilType = NONE);
			~FrameBuffer();

			GLuint setColorAttachment(AttachmentType type);
			GLuint setDepthAttachment(AttachmentType type);
			GLuint setStencilAttachment(AttachmentType type);

			void bind() const;
			void unbind() const;

			inline uint getWidth() const { return m_width; }
			inline uint getHeight() const { return m_height; }
			inline AttachmentType getColorType() const { return m_colorType; }
			inline AttachmentType getDepthType() const { return m_depthType; }
			inline AttachmentType getStencilType() const { return m_stencilType; }
			inline GLuint getColorID() const { return m_colorID; }
			inline GLuint getDepthID() const { return m_depthID; }
			inline GLuint getStencilID() const { return m_stencilID; }
			Texture getColorTexture() const;
			Texture getDepthTexture() const;
			Texture getStencilTexture() const;

			void setSize(uint width, uint height);
			void setScreenSize(uint screenW, uint screenH);
		};
	}
}