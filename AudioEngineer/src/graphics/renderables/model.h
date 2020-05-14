#pragma once

#include <vector>
#include <GL\glew.h>
#include "..\..\defines.h"

namespace audioengineer {
	namespace graphics {

		class Model
		{
		private:
			GLuint m_vaID;
			GLuint m_vbID;
			GLuint m_nbID;
			GLuint m_ubID;
			GLuint m_ibID;
			uint m_componentCount;
			uint m_vertexCount;
			uint m_indexCount;

		public:
			static Model* quad;

		public:
			Model(uint componentCount);
			~Model();

			void enable() const;
			void disable() const;

			void storeVertices(const std::vector<float>& vertices);
			void storeNormals(const std::vector<float>& normals);
			void storeUVs(const std::vector<float>& uvs);
			void storeIndices(const std::vector<uint>& indices);

			inline GLuint getVertexArrayID() const { return m_vaID; }
			inline GLuint getVertexBufferID() const { return m_vbID; }
			inline GLuint getNormalBufferID() const { return m_nbID; }
			inline GLuint getUVBufferID() const { return m_ubID; }
			inline GLuint getIndexBufferID() const { return m_ibID; }
			inline uint getComponentCount() const{ return m_componentCount; }
			inline uint getVertexCount() const{ return m_vertexCount; }
			inline uint getIndexCount() const { return m_indexCount; }
		};
	}
}