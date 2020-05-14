#pragma once

#include <GL\glew.h>
#include "arraybuffer.h"
#include "indexbuffer.h"
#include "..\..\defines.h"

namespace audioengineer {
	namespace graphics {

		class VertexArray
		{
		private:
			GLuint m_ID;
			std::vector<ArrayBuffer*> m_buffers;
			IndexBuffer* m_indices;

		public:
			VertexArray();
			~VertexArray();

			void bind() const;
			void unbind() const;

			void enable() const;
			void disable() const;

			void vertexAttribPointer(ArrayBuffer& buffer);
			void setIndexBuffer(IndexBuffer& indexBuffer);

			inline GLuint getID() const { return m_ID; }
			const std::vector<ArrayBuffer*>& getBuffers() const { return m_buffers; }
			IndexBuffer* getIndexBuffer() const { return m_indices; }
		};
	}
}