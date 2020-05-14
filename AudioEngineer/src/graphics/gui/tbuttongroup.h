#pragma once

#include <vector>
#include <initializer_list>
#include "togglebutton.h"

namespace audioengineer {
	namespace graphics {

		class TButtonGroup
		{
		private:
			std::vector<ToggleButton*> m_tbuttonList;
			EventHandler* m_eventHandler;

		public:
			TButtonGroup(EventHandler* eventHandler, const std::initializer_list<ToggleButton*>& tbuttonList = {});
			~TButtonGroup();

			void addToggleButton(ToggleButton* toggleButton);
			void removeToggleButton(ToggleButton* toggleButton);

			void turnAllOff() const;

		private:
			void buttonPressed(GuiEvent& ge);
		};
	}
}