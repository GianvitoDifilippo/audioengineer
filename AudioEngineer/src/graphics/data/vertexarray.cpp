#include "vertexarray.h"

namespace audioengineer {
	namespace graphics {

		VertexArray::VertexArray()
		{
			glGenVertexArrays(1, &m_ID);
			m_indices = nullptr;
		}

		VertexArray::~VertexArray()
		{
			glDeleteVertexArrays(1, &m_ID);
		}

		void VertexArray::bind() const
		{
			glBindVertexArray(m_ID);
		}

		void VertexArray::unbind() const
		{
			glBindVertexArray(0);
		}

		void VertexArray::enable() const
		{
			glBindVertexArray(m_ID);
			for (uint i = 0; i < m_buffers.size(); i++) {
				glEnableVertexAttribArray(i);
			}
			if (m_indices) m_indices->bind();
		}

		void VertexArray::disable() const
		{
			if (m_indices) m_indices->unbind();
			for (uint i = 0; i < m_buffers.size(); i++) {
				glDisableVertexAttribArray(i);
			}
			glBindVertexArray(0);
		}

		void VertexArray::vertexAttribPointer(ArrayBuffer& buffer)
		{
			glBindVertexArray(m_ID);
			glBindBuffer(GL_ARRAY_BUFFER, buffer.getID());
			glVertexAttribPointer(m_buffers.size(), buffer.getComponentCount(), GL_FLOAT, GL_FALSE, 0, nullptr);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			m_buffers.push_back(&buffer);
		}

		void VertexArray::setIndexBuffer(IndexBuffer& indexBuffer)
		{
			m_indices = &indexBuffer;
		}
	}
}