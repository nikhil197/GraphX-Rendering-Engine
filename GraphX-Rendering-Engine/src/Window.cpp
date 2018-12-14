#include "pch.h"
#include "Window.h"

#include "GLFW/glfw3.h"
#include "timer/Timer.h"
#include "gui/GraphXGui.h"
#include "events/WindowEvent.h"

namespace engine
{
	Window::Window(std::string title, int width, int height)
		: m_Title(title), m_Width(width), m_Height(height), m_EventCallback(nullptr)
	{
		bool res = Init();
		if (!res)
		{
			GX_ENGINE_ERROR("Window: Error While creating window");
		}

		/* Intialise the ImGui */
		GraphXGui::Init(m_Window);
		GX_ENGINE_INFO("Window: Successfully Created window");
	}

	bool Window::Init()
	{
		// To time the intialising sequence
		Timer windowInit("Window Intialising");

		if (!glfwInit())
		{
			GX_ENGINE_ERROR("Couldn't Initialise GLFW");
			return false;
		}

		// Set the context version and the core profile for the opengl
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Create the window
		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);

		if (!m_Window)
		{
			GX_ENGINE_ERROR("Window: Couldn't Create the GLFW window");
			glfwTerminate();
			return false;
		}

		// Make the context current
		glfwMakeContextCurrent(m_Window);

		// Setting the swap interval
		glfwSwapInterval(1);

		// Set the error callback for glfw
		glfwSetErrorCallback(&GlfwErrorCallback);

		// Set the user window pointer to this
		glfwSetWindowUserPointer(m_Window, this);

		// Here, for the time being static cast (not good) is being used to retrieve the window object from the GLFW,
		// since the capturing lambdas cannot be converted to function pointers
		// Need to find a better way!!!!!!

		// Frame Buffer resize callback
		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->FrameBufferSizeCallback(window, width, height);
		});

		// Window Move callback
		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->WindowPositionCallback(window, xpos, ypos);
		});

		// Window focus callback
		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->WindowFocusCallback(window, focused);
		});

		// Window close callback
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->WindowCloseCallback(window);
		});

		// Key callback
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int keyCode, int scanCode, int action, int mods) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->KeyCallback(window, keyCode, scanCode, action, mods);
		});

		// Cursor callback
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->CursorPositionCallback(window, xpos, ypos);
		});

		// Mouse button callback
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->MouseButtonCallback(window, button, action, mods);
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
			win->ScrollCallback(window, xOffset, yOffset);
		});

		return true;
	}

	void Window::OnEvent(Event& e)
	{
		bool handled = false;
		EventDispatcher dispatcher(e);
		if (e.IsInCategory(GX_EVENT_CATEGORY_WINDOW))
		{
			// handle the window events
		}
		else
		{
			// Dispatch the event to the application
			handled = m_EventCallback(e);
		}

		// Raise an error
		if (!handled)
		{
			GX_ENGINE_ERROR("\"{0}\"  could not be handled properly", e);
		}
	}

	bool Window::IsClosed() const
	{
		if (m_Window)
		{
			if (!glfwWindowShouldClose(m_Window))
			{
				return false;
			}
		}

		return true;
	}

	void Window::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void Window::SetEventCallback(std::function<bool(Event&)>& func)
	{
		m_EventCallback = func;
	}

	void Window::Resize()
	{
		int width, height;

		// Get the new width and height of the window
		glfwGetFramebufferSize(m_Window, &width, &height);

		// Set the new size of the window
		glViewport(0, 0, width, height);
	}

	void Window::Clear()
	{
		/* Clear both color and depth buffer */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// More efficient to do in one function call
	}

	void Window::Update()
	{
		/* Swap the front and back buffers */
		glfwSwapBuffers(m_Window);

		/* Poll for events */
		glfwPollEvents();
	}

	Window::~Window()
	{
		GX_ENGINE_INFO("Window: Destroying window");

		/* Cleanup the ImGui */
		GraphXGui::Cleanup();

		/* Destroy the window */
		glfwDestroyWindow(m_Window);

		/* Terminate */
		glfwTerminate();
	}

#pragma region callbacks

	/************* Window Callbacks **************/
	void Window::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{

	}

	void Window::WindowPositionCallback(GLFWwindow* window, int xpos, int ypos)
	{

	}

	void Window::WindowFocusCallback(GLFWwindow* window, int focused)
	{

	}

	void Window::WindowCloseCallback(GLFWwindow* window)
	{

	}

	/************* Input Callbacks **************/
	void Window::KeyCallback(GLFWwindow* window, int keyCode, int scanCode, int action, int mods)
	{

	}

	void Window::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{

	}

	void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{

	}

	void Window::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{

	}

#pragma endregion

#pragma region eventHandlers

	bool Window::OnWindowResize(WindowResizedEvent& e)
	{
		return false;
	}

	bool Window::OnWindowMoved(WindowMovedEvent& e)
	{
		return false;
	}

	bool Window::OnWindowFocus(WindowFocusEvent& e)
	{
		return false;
	}

	bool Window::OnWindowLostFocus(WindowLostFocusEvent& e)
	{
		return false;
	}

	bool Window::OnWindowClose(WindowCloseEvent& e)
	{
		return false;
	}

#pragma endregion

}