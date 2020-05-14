#include "button.h"

namespace audioengineer {
	namespace graphics {

		Button::Button()
			: Component(0, 0)
		{
			m_eventHandler = EventHandler::getDefaultHandler();
			m_eventHandler->addEventListener(&Button::click, this);
		}

		Button::Button(Texture* texture)
			: Component(0, 0)
		{
			m_texture = texture;
			m_eventHandler = EventHandler::getDefaultHandler();
			m_eventHandler->addEventListener(&Button::click, this);
		}

 		Button::~Button()
 		{
 			// delete m_text;
			m_eventHandler->removeEventListener(&Button::click, this);
 		}

		void Button::click()
		{
			m_eventHandler->submitEvent(GuiEvent(this, BUTTON_CLICK));
		}

		void Button::setEventHandler(EventHandler* eventHandler)
		{
			m_eventHandler->removeEventListener(&Button::click, this);
			m_eventHandler = eventHandler;
			m_eventHandler->addEventListener(&Button::click, this);
		}

		void Button::setTexture(Texture* texture)
		{
			m_texture = texture;
		}

		void Button::click(MouseEvent& me)
		{
			if (m_eventHandler && me.getButton() == GLFW_MOUSE_BUTTON_1 && m_enabled) {
				int x = m_eventHandler->getMouseX();
				int y = m_eventHandler->getMouseY();

				if (contains(x, y)) {
					if (me.getAction() == GLFW_PRESS) {
						m_pressed = true;
						m_eventHandler->submitEvent(GuiEvent(this, BUTTON_CLICK));
					}
				}
				if (me.getAction() == GLFW_RELEASE) {
					m_pressed = false;
				}
			}
		}
	}
}