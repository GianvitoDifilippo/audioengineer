#pragma once

#include "button.h"

namespace audioengineer {
	namespace graphics {

		class ToggleButton : public Button
		{
		private:
			Texture* m_pressedTexture;

		public:
			ToggleButton();
			ToggleButton(Texture* texture, Texture* pressedTexture);
			// ToggleButton(const Text& text);
			// ToggleButton(Texture* texture, Texture* pressedTexture, const Text& text);

			void setPressedTexture(Texture* texture);
			void setPressed(bool pressed);

			inline Texture* getPressedTexture() const { return m_pressedTexture; }

			inline Texture* getImage() const override { return m_pressed ? m_pressedTexture : m_texture; }

		private:
			void click(MouseEvent& me) override;
		};

	}
}