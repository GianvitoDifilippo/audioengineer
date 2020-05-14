#pragma once

#include "event.h"

namespace audioengineer {

	class MouseEvent : public Event
	{
	private:
		int m_button;
		int m_action;

	public:
		MouseEvent(int button, int action)
			: m_button(button), m_action(action) { }

		inline int getButton() const { return m_button; }
		inline int getAction() const { return m_action; }

		inline std::type_index getID() const override { return typeid(MouseEvent); }
	};
}