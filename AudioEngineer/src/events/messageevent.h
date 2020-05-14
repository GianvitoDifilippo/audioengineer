#pragma once

#include "event.h"
#include <string>

namespace audioengineer {

	class MessageEvent : public Event
	{
	private:
		std::string m_message;

	public:
		MessageEvent(const std::string& message)
			: m_message(message) { }

		inline const std::string& getMessage() const { return m_message; }

		inline std::type_index getID() const override { return typeid(MessageEvent); }
	};
}