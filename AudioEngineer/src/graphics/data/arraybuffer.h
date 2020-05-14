#pragma once

#include <vector>
#include <GL\glew.h>
#include "..\..\defines.h"

namespace audioengineer {
	namespace graphics {

		class ArrayBuffer
		{
		private:
			GLuint m_ID;
			size_t m_size;
			uint m_componentCount;

		public:
			ArrayBuffer(uint componentCount);
			~ArrayBuffer();

			void bind() const;
			void unbind() const;
			void bufferData(const std::vector<float>& data, GLenum usage);
			void bufferData(float* data, size_t size, GLenum usage);

			inline GLuint getID() const { return m_ID; }
			inline size_t getSize() const { return m_size; }
			inline uint getComponentCount() const { return m_componentCount; }
		};
	}
}