#include "eventhandler.h"

namespace audioengineer {

	EventHandler EventHandler::defaultHandler;

	void EventHandler::setMousePosition(double mouseX, double mouseY)
	{
		m_mouseX = (int)mouseX;
		m_mouseY = (int)mouseY;
	}

	void EventHandler::submitEvent(Event& event)
	{
		std::type_index eventID = event.getID();
		for (EventListenerBase* receiver : m_listeners) {
			if (eventID == receiver->getEventID()) {
				receiver->receive(event);
			}
		}
		if (eventID == typeid(MouseEvent)) {
			MouseEvent& mouseEvent = static_cast<MouseEvent&>(event);
			int action = mouseEvent.getAction();
			if (action == GLFW_PRESS) {
				m_buttons[mouseEvent.getButton()] = true;
			}
			else if (action == GLFW_RELEASE) {
				m_buttons[mouseEvent.getButton()] = false;
			}
		}
		else if (eventID == typeid(KeyboardEvent)) {
			KeyboardEvent& keyboardEvent = static_cast<KeyboardEvent&>(event);
			int action = keyboardEvent.getAction();
			if (action == GLFW_PRESS) {
				m_keys[keyboardEvent.getButton()] = true;
			}
			else if (action == GLFW_RELEASE) {
				m_keys[keyboardEvent.getButton()] = false;
			}
		}
	}

	void EventHandler::addEventListener(EventListenerBase* listener)
	{
		m_listeners.push_back(listener);
	}

	void EventHandler::removeEventListener(EventListenerBase* listener)
	{
		m_listeners.erase(std::remove(m_listeners.begin(), m_listeners.end(), listener), m_listeners.end());
	}

	EventHandler* EventHandler::getDefaultHandler()
	{
		return &defaultHandler;
	}
}