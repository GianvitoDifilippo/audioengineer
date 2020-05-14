#pragma once

#include "shader.h"

namespace audioengineer {
	namespace graphics {

		class GraphShader : public Shader
		{
		private:
			GLuint fs_location_graphColor;

		public:
			GraphShader();

			void loadColor(const maths::vec4& color);

		private:
			void getAllUniformLocations() override;
		};
	}
}