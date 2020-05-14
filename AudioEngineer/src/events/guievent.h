#pragma once

#include <type_traits>
#include "event.h"
#include "..\graphics\gui\component.h"

namespace audioengineer {

	enum GuiEventDescription
	{
		BUTTON_CLICK,
		TBUTTON_CLICK,
		TBUTTON_ON,
		TBUTTON_OFF
	};

	class GuiEvent : public Event
	{
	private:
		graphics::Component* m_component;
		GuiEventDescription m_description;

	public:
		GuiEvent(graphics::Component* component, GuiEventDescription description)
			: m_component(component), m_description(description) { }

		inline graphics::Component* getComponent() const { return m_component; }
		inline GuiEventDescription getDescription() const { return m_description; }
		inline std::type_index getID() const override { return typeid(GuiEvent); }
		
	};
}