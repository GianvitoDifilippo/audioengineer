#pragma once

#include <GL\glew.h>
#include <map>
#include "..\..\maths\vec4.h"
#include "..\..\utils\fileutils.h"
#include "..\..\defines.h"

namespace audioengineer {
	namespace graphics {

		class Texture
		{
		private:
			GLuint m_ID;
			uint m_width;
			uint m_height;

		public:
			static std::map<std::string, Texture*> textures;

		public:
			Texture(uint color);
			Texture(const maths::vec4& color);
			Texture(const std::string& filename, bool interpolate = false);
			Texture(GLuint ID, uint width, uint height);
			~Texture();

			void bind() const;
			void unbind() const;

			void setID(GLuint ID);
			void setWidth(uint width);
			void setHeight(uint height);

			inline GLuint getID() const { return m_ID; }
			inline uint getWidth() const { return m_width; }
			inline uint getHeight() const { return m_height; }

			static Texture* loadTexture(const std::string& name, uint color);
			static Texture* loadTexture(const std::string& name, const maths::vec4& color);
			static Texture* loadTexture(const std::string& name, const std::string& filename, bool interpolate = false);

			static Texture* getTexture(const std::string& name);
		};
	}
}