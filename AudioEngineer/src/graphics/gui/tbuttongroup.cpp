#include "tbuttongroup.h"

namespace audioengineer {
	namespace graphics {

		TButtonGroup::TButtonGroup(EventHandler* eventHandler, const std::initializer_list<ToggleButton*>& tbuttonList)
		{
			m_eventHandler = EventHandler::getDefaultHandler();
			m_eventHandler->addEventListener(&TButtonGroup::buttonPressed, this);
			for (ToggleButton* tbutton : tbuttonList) {
				m_tbuttonList.push_back(tbutton);
			}
		}

		TButtonGroup::~TButtonGroup()
		{
			m_eventHandler->removeEventListener(&TButtonGroup::buttonPressed, this);
		}

		void TButtonGroup::addToggleButton(ToggleButton* toggleButton)
		{
			m_tbuttonList.push_back(toggleButton);
		}

		void TButtonGroup::removeToggleButton(ToggleButton* toggleButton)
		{
			m_tbuttonList.erase(std::remove(m_tbuttonList.begin(), m_tbuttonList.end(), toggleButton), m_tbuttonList.end());
		}

		void TButtonGroup::turnAllOff() const
		{
			for (ToggleButton* tb : m_tbuttonList) {
				tb->setPressed(false);
			}
		}

		void TButtonGroup::buttonPressed(GuiEvent& ge)
		{
			if (ge.getDescription() == TBUTTON_CLICK) {
				ToggleButton* toggleButton = static_cast<ToggleButton*>(ge.getComponent());
				bool found = false;
				for (ToggleButton* tb : m_tbuttonList) {
					if (toggleButton == tb) {
						found = true;
						break;
					}
				}
				for (ToggleButton* tb : m_tbuttonList) {
					if (tb != toggleButton) {
						if (tb->isPressed()) {
							tb->setPressed(false);
						}
					}
				}
			}
		}
	}
}