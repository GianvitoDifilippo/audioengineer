#pragma once

#include <string>
#include <GL\glew.h>
#include "..\..\defines.h"
#include "..\..\maths\maths.h"
#include "..\..\utils\fileutils.h"

namespace audioengineer {
	namespace graphics {

		class Shader
		{
		private:
			GLuint m_programID;
			GLuint m_vertexID;
			GLuint m_fragmentID;

		public:
			Shader(const char* vertPath, const char* fragPath);
			~Shader();

			void enable() const;
			void disable() const;

		protected:
			GLuint getUniformLocation(const char* name) const;
			virtual void getAllUniformLocations() = 0;

			void loadFloat(GLuint location, float f) const;
			void loadInt(GLuint location, int i) const;
			void loadBool(GLuint location, bool b) const;
			void loadVec2(GLuint location, const maths::vec2& vec) const;
			void loadVec3(GLuint location, const maths::vec3& vec) const;
			void loadVec4(GLuint location, const maths::vec4& vec) const;
			void loadMat2(GLuint location, const maths::mat2& mat) const;
			void loadMat3(GLuint location, const maths::mat3& mat) const;
			void loadMat4(GLuint location, const maths::mat4& mat) const;
		};
	}
}