#pragma once

#include "component.h"
#include "..\renderables\text.h"
#include "..\..\events\eventsystem.h"

namespace audioengineer {
	namespace graphics {

		class Button : public Component
		{
		protected:
			EventHandler* m_eventHandler;
			Texture* m_texture;
			//Text* m_text;
			bool m_pressed;

		public:
			Button();
			Button(Texture* texture);
			// Button(const Text& text);
			// Button(Texture* texture, const Text& text);
			~Button();

			void setEventHandler(EventHandler* eventHandler) override;
			void setTexture(Texture* texture);
			void click();
			// void setText(const Text& text);

			inline Texture* getTexture() const { return m_texture; }
			// inline Text* getText() const { return m_text; }
			inline bool isPressed() const { return m_pressed; }

			inline virtual Texture* getImage() const override { return m_texture; }

		private:
			inline uint getRealWidth() const override { return m_texture ? m_texture->getWidth() : 50; }
			inline uint getRealHeight() const override { return m_texture ? m_texture->getHeight() : 50; }

			virtual void click(MouseEvent& me);
		};
	}
}