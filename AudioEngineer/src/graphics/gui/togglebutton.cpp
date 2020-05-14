#include "togglebutton.h"

namespace audioengineer {
	namespace graphics {

		ToggleButton::ToggleButton()
			: Button()
		{
			m_pressed = false;
		}

		ToggleButton::ToggleButton(Texture* texture, Texture* pressedTexture)
			: Button(texture)
		{
			m_pressedTexture = pressedTexture;
			m_pressed = false;
		}

// 		ToggleButton::ToggleButton(const Text& text)
// 			: Button(text)
// 		{
// 
// 		}

// 		ToggleButton::ToggleButton(Texture* texture, Texture* pressedTexture, const Text& text)
// 			: Button(texture, text)
// 		{
// 			m_pressedTexture = pressedTexture;
// 		}

		void ToggleButton::setPressedTexture(Texture* pressedTexture)
		{
			m_pressedTexture = pressedTexture;
		}

		void ToggleButton::setPressed(bool pressed)
		{
			m_pressed = pressed;
			if (m_pressed) {
				m_eventHandler->submitEvent(GuiEvent(this, TBUTTON_ON));
			}
			else {
				m_eventHandler->submitEvent(GuiEvent(this, TBUTTON_OFF));
			}
		}

		void ToggleButton::click(MouseEvent& me)
		{
			if (m_eventHandler && me.getButton() == GLFW_MOUSE_BUTTON_1 && m_enabled) {
				int x = m_eventHandler->getMouseX();
				int y = m_eventHandler->getMouseY();

				if (contains(x, y)) {
					if (me.getAction() == GLFW_PRESS) {
						m_eventHandler->submitEvent(GuiEvent(this, TBUTTON_CLICK));
						setPressed(!m_pressed);
					}
				}
			}
		}
	}
}