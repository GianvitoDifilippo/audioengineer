#pragma once

#include <vector>
#include <GL\glew.h>
#include "..\..\defines.h"

namespace audioengineer {
	namespace graphics {

		class IndexBuffer
		{
		private:
			uint m_ID;
			size_t m_size;

		public:
			IndexBuffer(const std::vector<uint>& indices);
			IndexBuffer(uint* indices, size_t size);
			~IndexBuffer();

			void bind() const;
			void unbind() const;

			size_t getSize() const { return m_size; }
		};
	}
}