#pragma once

#include "event.h"
#include "..\defines.h"

namespace audioengineer {

	class _StaticEventListenerType;

	class EventListenerBase
	{
	public:
		virtual void receive(Event& event) const = 0;
		virtual std::type_index getEventID() const = 0;
	};

}