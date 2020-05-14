#include "shader.h"

namespace audioengineer {
	namespace graphics {

		Shader::Shader(const char* vertPath, const char* fragPath)
		{
			std::string vertSource = utils::FileUtils::read_file(vertPath);
			std::string fragSource = utils::FileUtils::read_file(fragPath);
			const char* vertex = vertSource.c_str();
			const char* fragment = fragSource.c_str();

			m_programID = glCreateProgram();
			m_vertexID = glCreateShader(GL_VERTEX_SHADER);
			m_fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
			GLint flag;

			glShaderSource(m_vertexID, 1, &vertex, nullptr);
			glCompileShader(m_vertexID);
			glGetShaderiv(m_vertexID, GL_COMPILE_STATUS, &flag);
			if (flag == GL_FALSE) {
				GLint length;
				glGetShaderiv(m_vertexID, GL_INFO_LOG_LENGTH, &length);
				char* error = new char[length + 1];
				glGetShaderInfoLog(m_vertexID, length, &length, error);
				std::cout << "Could not compile the vertex shader: " << std::string(error) << std::endl;
				glDeleteShader(m_vertexID);
				glDeleteProgram(m_programID);
				m_vertexID = 0;
				m_programID = 0;
				delete[] error;
				return;
			}

			glShaderSource(m_fragmentID, 1, &fragment, nullptr);
			glCompileShader(m_fragmentID);
			glGetShaderiv(m_fragmentID, GL_COMPILE_STATUS, &flag);
			if (flag == GL_FALSE) {
				GLint length;
				glGetShaderiv(m_fragmentID, GL_INFO_LOG_LENGTH, &length);
				char* error = new char[length + 1];
				glGetShaderInfoLog(m_fragmentID, length, &length, error);
				std::cout << "Could not compile the vertex shader: " << std::string(error) << std::endl;
				glDeleteShader(m_vertexID);
				glDeleteShader(m_fragmentID);
				glDeleteProgram(m_programID);
				m_vertexID = 0;
				m_fragmentID = 0;
				m_programID = 0;
				delete[] error;
				return;
			}

			glAttachShader(m_programID, m_vertexID);
			glAttachShader(m_programID, m_fragmentID);
			glLinkProgram(m_programID);
			glValidateProgram(m_programID);
			glDeleteShader(m_vertexID);
			glDeleteShader(m_fragmentID);
		}

		Shader::~Shader()
		{
			glDeleteProgram(m_programID);
		}

		void Shader::enable() const
		{
			glUseProgram(m_programID);
		}

		void Shader::disable() const
		{
			glUseProgram(0);
		}

		GLuint Shader::getUniformLocation(const char* name) const
		{
			return glGetUniformLocation(m_programID, name);
		}

		void Shader::loadFloat(GLuint location, float f) const
		{
			glUniform1f(location, f);
		}

		void Shader::loadInt(GLuint location, int i) const
		{
			glUniform1i(location, i);
		}

		void Shader::loadBool(GLuint location, bool b) const
		{
			glUniform1f(location, b ? 1.f : 0.f);
		}

		void Shader::loadVec2(GLuint location, const maths::vec2& vec) const
		{
			glUniform2f(location, vec.x, vec.y);
		}

		void Shader::loadVec3(GLuint location, const maths::vec3& vec) const
		{
			glUniform3f(location, vec.x, vec.y, vec.z);
		}

		void Shader::loadVec4(GLuint location, const maths::vec4& vec) const
		{
			glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
		}

		void Shader::loadMat2(GLuint location, const maths::mat2& mat) const
		{
			glUniformMatrix2fv(location, 1, GL_TRUE, &mat.elements[0]);
		}

		void Shader::loadMat3(GLuint location, const maths::mat3& mat) const
		{
			glUniformMatrix3fv(location, 1, GL_TRUE, &mat.elements[0]);
		}

		void Shader::loadMat4(GLuint location, const maths::mat4& mat) const
		{
			glUniformMatrix4fv(location, 1, GL_TRUE, &mat.elements[0]);
		}
	}
}