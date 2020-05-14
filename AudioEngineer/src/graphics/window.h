#pragma once

#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "renderables\model.h"
#include "renderables\texture.h"
#include "..\defines.h"
#include "..\events\eventsystem.h"

#define WIN_FULLSCREEN		1 << 0
#define WIN_UNRESIZABLE		1 << 1
#define WIN_INVISIBLE		1 << 2
#define WIN_UNDECORATED		1 << 3

namespace audioengineer {
	namespace graphics {

		class Window
		{
		private:
			static GLFWwindow* m_window;
			static GLFWmonitor* m_monitor;
			static uint m_width;
			static uint m_height;
			static std::string m_title;
			static EventHandler* m_eventHandler;

		public:
			static void create(uint width, uint height, const std::string& title, int attribs = 0);
			static void destroy();

			static bool isOpen();
			static void update();
			static void clear();

			static void close();
			static void setEventHandler(EventHandler* eventHandler);

			static inline uint getWidth() { return m_width; }
			static inline uint getHeight() { return m_height; }
			static EventHandler* getEventHandler() { return m_eventHandler; }

			static void cursor_callback(GLFWwindow* window, double xpos, double ypos);
			static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
			static void resize_callback(GLFWwindow *window, int width, int height);

		private:
			Window();

			static void close(WinCloseEvent&, EventHandler&, void*);
		};
	}
}