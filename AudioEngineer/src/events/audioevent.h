#pragma once

#include <type_traits>
#include "event.h"
#include "..\graphics\gui\component.h"

namespace audioengineer {

	enum AudioEventDescription
	{
		WAVEFORM_RECORDED
	};

	class AudioEvent : public Event
	{
	private:
		AudioEventDescription m_description;

	public:
		AudioEvent(AudioEventDescription description)
			: m_description(description) { }

		inline AudioEventDescription getDescription() const { return m_description; }
		inline std::type_index getID() const override { return typeid(AudioEvent); }

	};
}