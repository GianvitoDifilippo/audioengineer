#include "scene.h"

namespace audioengineer {
	namespace graphics {

		Scene::Scene()
		{

		}

		void Scene::setVisible(bool visible)
		{
			for (Component* c : m_components) {
				c->setVisible(visible);
			}
		}

		void Scene::setEnabled(bool enabled)
		{
			for (Component* c : m_components) {
				c->setEnabled(enabled);
			}
		}

		void Scene::addComponent(Component* component)
		{
			m_components.push_back(component);
		}

		void Scene::removeComponent(const Component* component)
		{
			m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
		}
	}
}