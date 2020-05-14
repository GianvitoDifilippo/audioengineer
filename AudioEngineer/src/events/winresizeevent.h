#pragma once

#include "event.h"

namespace audioengineer {

	class WinResizeEvent : public Event
	{
	private:
		uint m_width;
		uint m_height;

	public:
		WinResizeEvent(uint width, uint height)
			: m_width(width), m_height(height) { }

		uint getWidth() const { return m_width; }
		uint getHeight() const { return m_height; }

		inline std::type_index getID() const override { return typeid(WinResizeEvent); }
	};
}