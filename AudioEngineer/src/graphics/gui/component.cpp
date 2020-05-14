#include "component.h"

namespace audioengineer {
	namespace graphics {

		Component::Component(int x, int y)
		{
			m_x = x;
			m_y = y;
			m_scaleX = 1.f;
			m_scaleY = 1.f;
			m_parent = nullptr;
			m_layer = 0;
			m_visible = true;
			m_enabled = true;
			m_alpha = 1.f;
		}

		void Component::setX(int x)
		{
			m_x = x;
		}

		void Component::setY(int y)
		{
			m_y = y;
		}

		void Component::setPosition(int x, int y)
		{
			m_x = x;
			m_y = y;
		}
		
		void Component::scale(float scaleX, float scaleY)
		{
			m_scaleX = scaleX;
			m_scaleY = scaleY;
		}

		void Component::setLayer(int layer)
		{
			m_layer = layer;
		}

		void Component::setRelativeTo(Component* parent)
		{
			m_parent = parent;
		}

		void Component::setVisible(bool visible)
		{
			m_visible = visible;
		}

		void Component::setEnabled(bool enabled)
		{
			m_enabled = enabled;
		}

		void Component::setAlpha(float alpha)
		{
			m_alpha = alpha;
		}

		bool Component::contains(int x, int y) const
		{
			return getX() <= x && x < getX() + (int)getWidth() && getY() <= y && y < getY() + (int)getHeight();
		}
	}
}