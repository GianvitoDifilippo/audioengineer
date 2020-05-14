#pragma once

#include "..\shaders\guishader.h"
#include "..\gui\scene.h"
#include "..\renderables\model.h"
#include "..\..\defines.h"
#include "..\..\events\eventsystem.h"

namespace audioengineer {
	namespace graphics {

		class GuiRenderer
		{
		private:
			GuiShader* m_shader;
			float m_scale;

		public:
			GuiRenderer(uint screenWidth, uint screenHeight);
			~GuiRenderer();

			void setScreenSize(uint width, uint height);
			void setScale(float scale);

			void render(const Scene* scene);

			inline float getScale() const { return m_scale; }
		};
	}
}