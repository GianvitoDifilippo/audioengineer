#include "window.h"

namespace audioengineer {
	namespace graphics {

		GLFWwindow* Window::m_window;
		GLFWmonitor* Window::m_monitor;
		uint Window::m_width;
		uint Window::m_height;
		std::string Window::m_title;
		EventHandler* Window::m_eventHandler;

		void Window::create(uint width, uint height, const std::string& title, int attribs)
		{
			m_window = nullptr;
			m_title = title;
			m_eventHandler = EventHandler::getDefaultHandler();
			m_eventHandler->addEventListener(Window::close);

			if (glfwInit() != GLFW_TRUE) {
				std::cout << "Could not initialize GLFW" << std::endl;
				return;
			}

			m_monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);
			m_width = width == 0 || width > (uint)mode->width ? (uint)mode->width : width;
			m_height = height == 0 || height > (uint)mode->height ? (uint)mode->height : height;
			glfwWindowHint(GLFW_RESIZABLE, attribs & (1 << 1) ? GLFW_FALSE : GLFW_TRUE);
			glfwWindowHint(GLFW_VISIBLE, attribs & (1 << 2) ? GLFW_FALSE : GLFW_TRUE);
			glfwWindowHint(GLFW_DECORATED, attribs & (1 << 3) ? GLFW_FALSE : GLFW_TRUE);
			m_window = glfwCreateWindow(m_width, m_height, title.c_str(), attribs & (1 << 0) ? m_monitor : nullptr, nullptr);
			if (m_window == nullptr) {
				std::cout << "Could not create a GLFWwindow" << std::endl;
				return;
			}
			glfwMakeContextCurrent(m_window);
			glfwSetCursorPosCallback(m_window, cursor_callback);
			glfwSetKeyCallback(m_window, key_callback);
			glfwSetMouseButtonCallback(m_window, mouse_callback);
			glfwSetWindowSizeCallback(m_window, resize_callback);

			if (glewInit() != GLEW_OK) {
				std::cout << "Could not initialize GLEW" << std::endl;
				return;
			}
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			std::vector<float> vertices = { 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 0.f };
			std::vector<GLuint> indices = { 0, 1, 2, 2, 3, 0 };
			Model::quad = new Model(2);
			Model::quad->storeVertices(vertices);
			Model::quad->storeIndices(indices);
		}

		void Window::destroy()
		{
			for (auto pair : Texture::textures)
				delete pair.second;
			delete Model::quad;
			m_eventHandler->removeEventListener(Window::close);
			glfwDestroyWindow(m_window);
			glfwTerminate();
		}

		bool Window::isOpen()
		{
			return !glfwWindowShouldClose(m_window);
		}

		void Window::update()
		{
			GLenum error = glGetError();
			glfwPollEvents();
			glfwSwapBuffers(m_window);
			if (m_eventHandler) {
				m_eventHandler->submitEvent(UpdateEvent());
				if (error != GLEW_NO_ERROR) {
					m_eventHandler->submitEvent(ErrorEvent("OpenGL error " + std::to_string(error)));
				}
			}
		}

		void Window::clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Window::close()
		{
			glfwSetWindowShouldClose(m_window, GLFW_TRUE);
		}

		void Window::close(WinCloseEvent&, EventHandler&, void*)
		{
			close();
		}

		void Window::setEventHandler(EventHandler* eventHandler)
		{
			if (m_eventHandler) {
				m_eventHandler->removeEventListener(Window::close);
			}
			m_eventHandler = eventHandler;
			if (m_eventHandler) {
				m_eventHandler->addEventListener(Window::close);
			}
		}

		void Window::cursor_callback(GLFWwindow* window, double x, double y)
		{
			if (m_eventHandler) {
				m_eventHandler->setMousePosition(x, y);
			}
		}

		void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			if (m_eventHandler) {
				m_eventHandler->submitEvent(KeyboardEvent(key, action));
			}
		}

		void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
		{
			if (m_eventHandler) {
				m_eventHandler->submitEvent(MouseEvent(button, action));
			}
		}

		void Window::resize_callback(GLFWwindow* window, int width, int height)
		{
			m_width = width;
			m_height = height;
			glViewport(0, 0, width, height);
			if (m_eventHandler) {
				m_eventHandler->submitEvent(WinResizeEvent((uint)width, (uint)height));
			}
		}

	}
}