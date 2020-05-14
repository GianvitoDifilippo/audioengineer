#pragma once

#include "event.h"

namespace audioengineer {

	class UpdateEvent : public Event
	{
	private:
		// timepassed

	public:
		UpdateEvent(/* timepassed */)
			/* timepassed */ { }

		// gettimepassed

		inline std::type_index getID() const override { return typeid(UpdateEvent); }
	};
}