#pragma once

#include "component.h"

namespace audioengineer {
	namespace graphics {

		class Panel : public Component
		{
		protected:
			Texture* m_texture;

		public:
			Panel();
			Panel(Texture* m_texture);

			void setTexture(Texture* texture);
			void setEventHandler(EventHandler* eventHandler);

			inline Texture* getTexture() const { return m_texture; }

			inline virtual Texture* getImage() const override { return m_texture; }

		private:
			inline uint getRealWidth() const override { return m_texture ? m_texture->getWidth() : 50; }
			inline uint getRealHeight() const override { return m_texture ? m_texture->getHeight() : 50; }
		};
	}
}