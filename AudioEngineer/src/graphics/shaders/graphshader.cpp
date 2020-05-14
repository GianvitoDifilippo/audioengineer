#include "graphshader.h"

namespace audioengineer {
	namespace graphics {

		GraphShader::GraphShader()
			: Shader("src/graphics/shadersources/graphshader.vert", "src/graphics/shadersources/graphshader.frag")
		{
			getAllUniformLocations();
		}

		void GraphShader::loadColor(const maths::vec4& color)
		{
			loadVec4(fs_location_graphColor, color);
		}

		void GraphShader::getAllUniformLocations()
		{
			fs_location_graphColor = getUniformLocation("graphColor");
		}
	}
}