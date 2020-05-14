#pragma once

#include "eventlistener.h"
#include "eventlistenerstatic.h"
#include "eventhandler.h"

#include "audioevent.h"
#include "errorevent.h"
#include "guievent.h"
#include "keyboardevent.h"
#include "messageevent.h"
#include "mouseevent.h"
#include "updateevent.h"
#include "wincloseevent.h"
#include "winresizeevent.h"

namespace audioengineer {

	template<class T = _StaticEventListenerType> using AudioListener = EventListener<AudioEvent, T>;
	template<class T = _StaticEventListenerType> using ErrorListener = EventListener<ErrorEvent, T>;
	template<class T = _StaticEventListenerType> using GuiListener = EventListener<GuiEvent, T>;
	template<class T = _StaticEventListenerType> using KeyboardListener = EventListener<KeyboardEvent, T>;
	template<class T = _StaticEventListenerType> using MessageListener = EventListener<MessageEvent, T>;
	template<class T = _StaticEventListenerType> using MouseListener = EventListener<MouseEvent, T>;
	template<class T = _StaticEventListenerType> using UpdateListener = EventListener<UpdateEvent, T>;
	template<class T = _StaticEventListenerType> using WinCloseListener = EventListener<WinCloseEvent, T>;
	template<class T = _StaticEventListenerType> using WinResizeListener = EventListener<WinResizeEvent, T>;

}