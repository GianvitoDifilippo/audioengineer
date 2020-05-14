#include "gramshader.h"

namespace audioengineer {
	namespace graphics {

		GramShader::GramShader()
			: Shader("src/graphics/shadersources/gramshader.vert", "src/graphics/shadersources/gramshader.frag")
		{

		}

		void GramShader::getAllUniformLocations()
		{

		}
	}
}