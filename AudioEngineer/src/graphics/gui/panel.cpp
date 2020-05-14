#include "panel.h"

namespace audioengineer {
	namespace graphics {

		Panel::Panel()
			: Component(0, 0)
		{

		}

		Panel::Panel(Texture* texture)
			: Component(0, 0)
		{
			m_texture = texture;
		}

		void Panel::setTexture(Texture* texture)
		{
			m_texture = texture;
		}

		void Panel::setEventHandler(EventHandler* eventHandler)
		{

		}
	}
}