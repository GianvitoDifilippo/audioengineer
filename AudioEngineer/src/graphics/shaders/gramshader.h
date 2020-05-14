#pragma once

#include "shader.h"

namespace audioengineer {
	namespace graphics {

		class GramShader : public Shader
		{
		private:

		public:
			GramShader();

		private:
			void getAllUniformLocations() override;
		};
	}
}