#include "model.h"

namespace audioengineer {
	namespace graphics {

		Model* Model::quad = nullptr;

		Model::Model(uint componentCount)
		{
			glGenVertexArrays(1, &m_vaID);
			m_componentCount = componentCount;
			m_vbID = 0;
			m_nbID = 0;
			m_ubID = 0;
			m_ibID = 0;
		}

		Model::~Model()
		{
			glDeleteVertexArrays(1, &m_vaID);
			if (m_vbID != 0) glDeleteBuffers(1, &m_vbID);
			if (m_nbID != 0) glDeleteBuffers(1, &m_nbID);
			if (m_ubID != 0) glDeleteBuffers(1, &m_ubID);
			if (m_ibID != 0) glDeleteBuffers(1, &m_ibID);
		}

		void Model::enable() const
		{
			glBindVertexArray(m_vaID);
			if (m_vbID != 0) glEnableVertexAttribArray(0);
			if (m_nbID != 0) glEnableVertexAttribArray(1);
			if (m_ubID != 0) glEnableVertexAttribArray(2);
		}

		void Model::disable() const
		{
			if (m_ubID != 0) glDisableVertexAttribArray(2);
			if (m_nbID != 0) glDisableVertexAttribArray(1);
			if (m_vbID != 0) glDisableVertexAttribArray(0);
			glBindVertexArray(0);
		}

		void Model::storeVertices(const std::vector<float>& vertices)
		{
			m_vertexCount = vertices.size();
			glBindVertexArray(m_vaID);
			if (m_vbID != 0) glDeleteBuffers(1, &m_vbID);
			glGenBuffers(1, &m_vbID);
			glBindBuffer(GL_ARRAY_BUFFER, m_vbID);
			glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float), &vertices[0], GL_STATIC_DRAW);
			glVertexAttribPointer(0, m_componentCount, GL_FLOAT, GL_FALSE, 0, nullptr);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void Model::storeNormals(const std::vector<float>& normals)
		{
			glBindVertexArray(m_vaID);
			if (m_nbID != 0) glDeleteBuffers(1, &m_nbID);
			glGenBuffers(1, &m_nbID);
			glBindBuffer(GL_ARRAY_BUFFER, m_nbID);
			glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, m_componentCount, GL_FLOAT, GL_FALSE, 0, nullptr);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void Model::storeUVs(const std::vector<float>& uvs)
		{
			glBindVertexArray(m_vaID);
			if (m_ubID != 0) glDeleteBuffers(1, &m_ubID);
			glGenBuffers(1, &m_ubID);
			glBindBuffer(GL_ARRAY_BUFFER, m_ubID);
			glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), &uvs[0], GL_STATIC_DRAW);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		
		void Model::storeIndices(const std::vector<uint>& indices)
		{
			m_indexCount = indices.size();
			glBindVertexArray(m_vaID);
			if (m_ibID != 0) glDeleteBuffers(1, &m_ibID);
			glGenBuffers(1, &m_ibID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(uint), &indices[0], GL_STATIC_DRAW);
			glBindVertexArray(0);
		}
	}
}