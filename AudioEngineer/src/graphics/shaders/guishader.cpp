#include "guishader.h"

namespace audioengineer {
	namespace graphics {

		GuiShader::GuiShader()
			: Shader("src/graphics/shadersources/guishader.vert", "src/graphics/shadersources/guishader.frag")
		{
			getAllUniformLocations();
			enable();
			connectTextures();
			loadScreenSize(1.f, 1.f);
			loadAlpha(1.f);
			disable();
		}

		void GuiShader::loadPosition(float x, float y) const
		{
			loadVec2(vs_location_position, maths::vec2(x, y));
		}

		void GuiShader::loadScaling(float width, float height) const
		{
			loadVec2(vs_location_scaling, maths::vec2(width, height));
		}

		void GuiShader::loadScreenSize(float width, float height) const
		{
			loadVec2(vs_location_screenSize, maths::vec2(width, height));
		}

		void GuiShader::loadAlpha(float alpha) const
		{
			loadFloat(fs_location_alpha, alpha);
		}

		void GuiShader::connectTextures() const
		{
			loadInt(fs_location_texSlot, 0);
		}

		void GuiShader::getAllUniformLocations()
		{
			vs_location_position = getUniformLocation("position");
			vs_location_scaling = getUniformLocation("scaling");
			vs_location_screenSize = getUniformLocation("screenSize");

			fs_location_texSlot = getUniformLocation("texSlot");
			fs_location_alpha = getUniformLocation("alpha");
		}
	}
}