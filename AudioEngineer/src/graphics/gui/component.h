#pragma once

#include "..\renderables\texture.h"
#include "..\..\events\eventhandler.h"
#include "..\..\defines.h"

namespace audioengineer {
	namespace graphics {

		class Component
		{
		protected:
			Component* m_parent;
			int m_x;
			int m_y;
			float m_scaleX;
			float m_scaleY;
			int m_layer;
			bool m_visible;
			bool m_enabled;
			float m_alpha;

			Component(int x, int y);

		public:
			void setRelativeTo(Component* parent);
			void setX(int x);
			void setY(int y);
			void setPosition(int x, int y);
			void scale(float scaleX, float scaleY);
			void setLayer(int layer);
			void setVisible(bool visible);
			void setEnabled(bool enabled);
			void setAlpha(float alpha);
			virtual void setEventHandler(EventHandler* eventHandler) = 0;

			bool contains(int x, int y) const;

			inline Component* getParent() const { return m_parent; }
			inline virtual int getX() const { return m_x + (m_parent ? m_parent->getX() : 0); }
			inline virtual int getY() const { return m_y + (m_parent ? m_parent->getY() : 0); }
			inline virtual int getRelativeX() const { return m_x; }
			inline virtual int getRelativeY() const { return m_y; }
			inline uint getWidth() const { return (uint)(m_scaleX * getRealWidth()); }
			inline uint getHeight() const { return (uint)(m_scaleY * getRealHeight()); }
			inline int getLayer() const { return m_layer; }
			inline bool isVisible() const { return m_visible; }
			inline bool isEnabled() const { return m_enabled; }
			inline float getAlpha() const { return m_alpha; }
			virtual Texture* getImage() const = 0;
			
		private:
			virtual uint getRealWidth() const = 0;
			virtual uint getRealHeight() const = 0;
		};
	}
}