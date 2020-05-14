#pragma once

#include <type_traits>

#include "eventlistener.h"

namespace audioengineer {

	class EventHandler;

	template<class EventChild>
	class EventListener<EventChild, _StaticEventListenerType> : public EventListenerBase
	{
		static_assert(std::is_base_of<Event, EventChild>::value, "EventChild should be subclass of Event");

	public:
		typedef void (*EventFunc)(EventChild&, EventHandler&, void*);

	private:
		EventFunc m_func;
		EventHandler* m_eventHandler;
		void* m_userData;

	public:
		EventListener(EventFunc func, EventHandler& eventHandler, void* userData = nullptr)
			: m_func(func), m_eventHandler(&eventHandler), m_userData(userData) { }

		inline void setEventFunc(const EventFunc& func) { m_func = func; }
		inline void setEventHandler(EventHandler& eventHandler) { m_eventHandler = &eventHandler; }
		inline void setUserData(void* userData) { m_userData; }

		inline const EventFunc& getEventFunc() const { return m_func; }
		inline EventHandler* getEventHandler() const { return m_eventHandler; }
		inline void* getUserData() const { return m_userData; }

		inline void receive(Event& event) const override { m_func(static_cast<EventChild&>(event), *m_eventHandler, m_userData); }
		inline std::type_index getEventID() const override { return typeid(EventChild); }
	};
}