#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "eventlistener.h"
#include "eventlistenerstatic.h"
#include "keyboardevent.h"
#include "mouseevent.h"
#include "..\defines.h"

#define EVH_MAX_KEYS		GLFW_KEY_LAST + 1
#define EVH_MAX_BUTTONS		GLFW_MOUSE_BUTTON_LAST + 1

namespace audioengineer {

	class EventHandler
	{
	private:
		int m_mouseX;
		int m_mouseY;
		bool m_keys[EVH_MAX_KEYS];
		bool m_buttons[EVH_MAX_BUTTONS];
		std::vector<EventListenerBase*> m_listeners;

		static EventHandler defaultHandler;

	public:
		void setMousePosition(double mouseX, double mouseY);
		void submitEvent(Event& event);

		static EventHandler* getDefaultHandler();

		void addEventListener(EventListenerBase* listener);
		void removeEventListener(EventListenerBase* listener);

		template<class EventChild, class T>
		inline EventListener<EventChild, T>* addEventListener(void (T::*eventFunc)(EventChild& event), T* instance)
		{
			static_assert(std::is_base_of<Event, EventChild>::value, "EventChild should be a subclass of Event");
			EventListener<EventChild, T>* listener = new EventListener<EventChild, T>(eventFunc, instance);
			m_listeners.push_back(listener);
			return listener;
		}

		template<class EventChild, class T>
		inline void removeEventListener(void (T::*eventFunc)(EventChild&), T* instance)
		{
			static_assert(std::is_base_of<Event, EventChild>::value, "EventChild should be a subclass of Event");
			for (EventListenerBase* listener : m_listeners) {
				EventListener<EventChild, T>* target = dynamic_cast<EventListener<EventChild, T>*>(listener);
				if (target) {
					if (target->getEventFunc() == eventFunc && target->getInstance() == instance) {
						removeEventListener(listener);
						delete target;
						return;
					}
				}
			}
		}

		template<class EventChild>
		inline EventListener<EventChild>* addEventListener(void (*eventFunc)(EventChild&, EventHandler&, void*), void* userData = nullptr)
		{
			static_assert(std::is_base_of<Event, EventChild>::value, "EventChild should be a subclass of Event");
			EventListener<EventChild>* listener = new EventListener<EventChild>(eventFunc, *this, userData);
			m_listeners.push_back(listener);
			return listener;
		}

		template<class EventChild>
		inline void removeEventListener(void(*eventFunc)(EventChild&, EventHandler&, void*))
		{
			static_assert(std::is_base_of<Event, EventChild>::value, "EventChild should be a subclass of Event");
			for (EventListenerBase* listener : m_listeners) {
				EventListener<EventChild>* target = dynamic_cast<EventListener<EventChild>*>(listener);
				if (target) {
					if (target->getEventFunc() == eventFunc) {
						removeEventListener(listener);
						delete target;
						return;
					}
				}
			}
		}

		inline int getMouseX() const { return m_mouseX; }
		inline int getMouseY() const { return m_mouseY; }
		inline bool getKey(uint keyCode) const { return m_keys[keyCode]; }
		inline bool getButton(uint buttonCode) const { return m_buttons[buttonCode]; }
	};
}