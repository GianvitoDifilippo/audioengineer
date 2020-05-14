#pragma once

#include "event.h"
#include <string>

namespace audioengineer {

	class ErrorEvent : public Event
	{
	private:
		std::string m_description;

	public:
		ErrorEvent(const std::string& description)
			: m_description(description) { }

		inline const std::string& getDescription() const { return m_description; }

		inline std::type_index getID() const override { return typeid(ErrorEvent); }
	};
}