#pragma once

#include <vector>
#include <algorithm>
#include "component.h"

namespace audioengineer {
	namespace graphics {

		class Scene
		{
		private:
			std::vector<Component*> m_components;

		public:
			Scene();

			void setVisible(bool visible);
			void setEnabled(bool enabled);
			void addComponent(Component* component);
			void removeComponent(const Component* component);

			inline const std::vector<Component*>& getComponents() const { return m_components; }
		};
	}
}