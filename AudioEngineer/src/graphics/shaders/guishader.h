#pragma once

#include "shader.h"

namespace audioengineer {
	namespace graphics {

		class GuiShader : public Shader
		{
		private:
			GLuint vs_location_position;
			GLuint vs_location_scaling;
			GLuint vs_location_screenSize;

			GLuint fs_location_texSlot;
			GLuint fs_location_alpha;

		public:
			GuiShader();

			void loadPosition(float x, float y) const;
			void loadScaling(float width, float height) const;
			void loadScreenSize(float width, float height) const;
			void loadAlpha(float alpha) const;

		private:
			void connectTextures() const;
			void getAllUniformLocations() override;
		};
	}
}