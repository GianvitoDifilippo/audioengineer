#include "guirenderer.h"

namespace audioengineer {
	namespace graphics {

		GuiRenderer::GuiRenderer(uint screenWidth, uint screenHeight)
		{
			m_shader = new GuiShader();
			m_shader->enable();
			m_shader->loadScreenSize((float)screenWidth, (float)screenHeight);
			m_shader->disable();
			m_scale = 1.f;
		}

		GuiRenderer::~GuiRenderer()
		{
			delete m_shader;
		}

		void GuiRenderer::setScreenSize(uint width, uint height)
		{
			m_shader->enable();
			m_shader->loadScreenSize((float)width, (float)height);
			m_shader->disable();
		}

		void GuiRenderer::setScale(float scale)
		{
			m_scale = scale;
		}

		void GuiRenderer::render(const Scene* scene)
		{
			const std::vector<Component*>& components = scene->getComponents();
			uint indexCount = Model::quad->getIndexCount();
			m_shader->enable();
			Model::quad->enable();
			for (const Component* component : components) {
				if (!component->isVisible()) continue;
				m_shader->loadPosition((float)component->getX() * m_scale, (float)component->getY() * m_scale);
				m_shader->loadScaling((float)component->getWidth() * m_scale, (float)component->getHeight() * m_scale);
				m_shader->loadAlpha(component->getAlpha());
				Texture* texture = component->getImage();
				texture->bind();
				glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
				texture->unbind();
			}
			Model::quad->disable();
			m_shader->disable();
		}
	}
}