#pragma once

#include <typeindex>
#include "..\defines.h"

namespace audioengineer {

	class Event
	{
	public:
		virtual std::type_index getID() const = 0;
	};

}