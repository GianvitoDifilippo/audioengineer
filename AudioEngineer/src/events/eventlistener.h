#pragma once

#include "eventlistenerbase.h"

namespace audioengineer {

	template<class EventChild, class T = _StaticEventListenerType>
	class EventListener : public EventListenerBase
	{
		static_assert(std::is_base_of<Event, EventChild>::value, "EventChild should be subclass of Event");

	public:
		typedef void (T::*EventFunc)(EventChild&);

	private:
		EventFunc m_func;
		T* m_instance;

	public:
		EventListener(EventFunc func, T* instance)
			: m_func(func), m_instance(instance) { }

		inline void setEventFunc(const EventFunc& func) { m_func = func; }
		inline void setInstance(T* instance) { m_instance = instance; }

		inline const EventFunc& getEventFunc() { return m_func; }
		inline T* getInstance() { return m_instance; }

		inline void receive(Event& event) const override { (m_instance->*m_func)(static_cast<EventChild&>(event)); }
		inline std::type_index getEventID() const override { return typeid(EventChild); }
	};
}