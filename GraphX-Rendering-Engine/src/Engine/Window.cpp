#include "pch.h"
#include "Window.h"

#include "GLFW/glfw3.h"

#include "GraphicsContext.h"
#include "Timer/Timer.h"
#include "Gui/GraphXGui.h"
#include "Events/WindowEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#include "Utilities/EngineUtil.h"
namespace GraphX
{
	/* Error Callback for GLFW Window */
	static void GlfwErrorCallback(int error, const char* description)
	{
		GX_ENGINE_ERROR("[GFLW Error] : {0}, {1}", error, description);
	}

	Window::Window(std::string title, int width, int height)
		: m_Title(title), m_Width(width), m_Height(height), m_Closed(false), m_EventCallback(nullptr)
	{
		bool res = Init();
		if (!res)
		{
			GX_ASSERT(res, "Error While creating window");
		}

		/* Intialise the ImGui */
		GraphXGui::Init(m_Window, BIND_EVENT_FUNC(Window::OnEvent));
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

		// Requesting opengl debug context
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		// Create the window
		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);

		if (!m_Window)
		{
			GX_ENGINE_ERROR("Window: Couldn't Create the GLFW window");
			glfwTerminate();
			return false;
		}

		// Make the context current
		m_Context = new GraphicsContext(m_Window);

		bool success = m_Context->Init();
		GX_ASSERT(success, "Failed to Intialize Graphics Context (OpenGL)");

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
		// Dispatch the event to the application
		// Unhandled event error will be raised by the event handler
		if(m_EventCallback)
			m_EventCallback(e);
	}

	void Window::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void Window::SetEventCallback(const std::function<void(Event&)>& func)
	{
		m_EventCallback = func;
	}

	void Window::Clear()
	{
		/* Clear both color and depth buffer */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// More efficient to do in one function call
	}

	void Window::ClearDepthBuffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void Window::OnUpdate()
	{
		/* Swap the front and back buffers */
		glfwSwapBuffers(m_Window);

		/* Poll for events */
		glfwPollEvents();
	}

	void Window::OnResize()
	{
		glViewport(0, 0, m_Width, m_Height);
	}

	Window::~Window()
	{
		GX_ENGINE_INFO("Window: Destroying window");

		/* Destroy the Graphics Context */
		delete m_Context;

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
		// Set the width and height of the window
		m_Width = width;
		m_Height = height;

		WindowResizedEvent e(width, height);
		OnEvent(e);
	}

	void Window::WindowPositionCallback(GLFWwindow* window, int xpos, int ypos)
	{
		WindowMovedEvent e(xpos, ypos);
		OnEvent(e);
	}

	void Window::WindowFocusCallback(GLFWwindow* window, int focused)
	{
		if (focused)
		{
			WindowFocusEvent e;
			OnEvent(e);
		}
		else
		{
			WindowLostFocusEvent e;
			OnEvent(e);
		}
	}

	void Window::WindowCloseCallback(GLFWwindow* window)
	{
		m_Closed = true;

		WindowCloseEvent e;
		OnEvent(e);
	}

	/************* Input Callbacks **************/
	void Window::KeyCallback(GLFWwindow* window, int keyCode, int scanCode, int action, int mods)
	{
		static int repeatCount = 0;

		if (action == GLFW_PRESS)
		{
			repeatCount = 1;
			KeyPressedEvent e(keyCode, repeatCount);
			OnEvent(e);
		}
		else if (action == GLFW_RELEASE)
		{
			repeatCount = 0;
			KeyReleasedEvent e(keyCode);
			OnEvent(e);
		}
		else if (action == GLFW_REPEAT)
		{
			repeatCount++;
			KeyPressedEvent e(keyCode, repeatCount);
			OnEvent(e);
		}
	}

	void Window::CursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		MouseMovedEvent e((float)xpos, (float)ypos);
		OnEvent(e);
	}

	void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			MouseButtonPressedEvent e(button);
			OnEvent(e);
		}
		else if (action == GLFW_RELEASE)
		{
			MouseButtonReleasedEvent e(button);
			OnEvent(e);
		}
	}

	void Window::ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
	{
		MouseScrolledEvent e((float)xOffset, (float)yOffset);
		OnEvent(e);
	}

#pragma endregion
}