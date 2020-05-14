#include "texture.h"

namespace audioengineer {
	namespace graphics {

		std::map<std::string, Texture*> Texture::textures;

		Texture::Texture(uint color)
		{
			m_width = 1;
			m_height = 1;
			glGenTextures(1, &m_ID);
			glBindTexture(GL_TEXTURE_2D, m_ID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, 1, 1, 0, GL_BGRA, GL_UNSIGNED_BYTE, &color);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		Texture::Texture(const maths::vec4& color)
		{
			m_width = 1;
			m_height = 1;
			glGenTextures(1, &m_ID);
			glBindTexture(GL_TEXTURE_2D, m_ID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, 1, 1, 0, GL_BGRA, GL_UNSIGNED_BYTE, &color);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		Texture::Texture(const std::string& filename, bool interpolate)
		{
			glGenTextures(1, &m_ID);
			glBindTexture(GL_TEXTURE_2D, m_ID);
			uint channels;
			BYTE* pixels = utils::FileUtils::read_image(filename, &m_width, &m_height, &channels);
			GLenum format = channels == 4 ? GL_BGRA : GL_BGR;
			if (!pixels) {
				uint black = 0;
				m_width = 1;
				m_height = 1;
				glTexImage2D(GL_TEXTURE_2D, 0, GL_BGRA, 1, 1, 0, GL_BGRA, GL_UNSIGNED_BYTE, &black);
			}
			else {
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolate ? GL_LINEAR : GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolate ? GL_LINEAR : GL_NEAREST);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, format , GL_UNSIGNED_BYTE, pixels);
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		Texture::Texture(GLuint ID, uint width, uint height)
		{
			m_ID = ID;
			m_width = width;
			m_height = height;
		}

		Texture::~Texture()
		{
			glDeleteTextures(1, &m_ID);
		}

		void Texture::setID(GLuint ID)
		{
			m_ID = ID;
		}

		void Texture::setWidth(uint width)
		{
			m_width = width;
		}

		void Texture::setHeight(uint height)
		{
			m_height = height;
		}

		void Texture::bind() const
		{
			glBindTexture(GL_TEXTURE_2D, m_ID);
		}

		void Texture::unbind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		Texture* Texture::loadTexture(const std::string& name, uint color)
		{
			Texture* result = new Texture(color);
			textures[name] = result;
			return result;
		}

		Texture* Texture::loadTexture(const std::string& name, const maths::vec4& color)
		{
			Texture* result = new Texture(color);
			textures[name] = result;
			return result;
		}

		Texture* Texture::loadTexture(const std::string& name, const std::string& filename, bool interpolate)
		{
			Texture* result = new Texture(filename, interpolate);
			textures[name] = result;
			return result;
		}

		Texture* Texture::getTexture(const std::string& name)
		{
			return textures[name];
		}
	}
}