#include "arraybuffer.h"

namespace audioengineer {
	namespace graphics {

		ArrayBuffer::ArrayBuffer(uint componentCount)
		{
			glGenBuffers(1, &m_ID);
			m_componentCount = componentCount;
		}

		ArrayBuffer::~ArrayBuffer()
		{
			glDeleteBuffers(1, &m_ID);
		}

		void ArrayBuffer::bind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		}

		void ArrayBuffer::unbind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void ArrayBuffer::bufferData(const std::vector<float>& data, GLenum usage)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], usage);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_size = data.size() / m_componentCount;
		}

		void ArrayBuffer::bufferData(float* data, size_t size, GLenum usage)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, size* sizeof(float), data, usage);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_size = size / m_componentCount;
		}
	}
}