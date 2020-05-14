#pragma once

#include "event.h"

namespace audioengineer {

	class WinCloseEvent : public Event
	{

	public:
		WinCloseEvent()
			 { }

		inline std::type_index getID() const override { return typeid(WinCloseEvent); }
	};
}