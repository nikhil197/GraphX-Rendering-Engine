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

	Window::Window(const WindowProps& props)
	{
		GX_PROFILE_FUNCTION()

		Init(props);
		
		GX_ENGINE_INFO("Window: Successfully Created window");
	}

	void Window::Init(const WindowProps& props)
	{
		GX_ENGINE_INFO("Intialising Window");
		
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		{
			GX_PROFILE_SCOPE("glfwInit()")

			if (!glfwInit())
			{
				GX_ENGINE_ASSERT(false, "Couldn't Initialise GLFW");
			}
		}

		{
			GX_PROFILE_SCOPE("glfw Initialise")

			// Requesting opengl debug context
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

			// Create the window
			m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), NULL, NULL);

			if (!m_Window)
			{
				GX_ENGINE_ASSERT(false, "Window: Couldn't Create the GLFW window");
				glfwTerminate();
			}

			// Make the context current
			m_Context = new GraphicsContext(m_Window);
			m_Context->Init();
		}

		SetVSync(false);

		// Set the error callback for glfw
		glfwSetErrorCallback(&GlfwErrorCallback);

		// Set the user window pointer to this
		glfwSetWindowUserPointer(m_Window, &m_Data);

		// Here, for the time being static cast (not good) is being used to retrieve the window object from the GLFW,
		// since the capturing lambdas cannot be converted to function pointers
		// Need to find a better way!!!!!!

		/************* Window Callbacks **************/

		// Frame Buffer resize callback
		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			// Set the width and height of the window
			data.Width = width;
			data.Height = height;

			WindowResizedEvent event(width, height);
			data.EventCallback(event);
		});

		// Window Move callback
		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			
			WindowMovedEvent event(xpos, ypos);
			data.EventCallback(event);
		});

		// Window focus callback
		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			if (focused)
			{
				WindowFocusEvent event;
				data.EventCallback(event);
			}
			else
			{
				WindowLostFocusEvent event;
				data.EventCallback(event);
			}
		});

		// Window close callback
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			WindowCloseEvent event;
			data.EventCallback(event);
		});

		/************** Input Callbacks ********************/

		// Key callback
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int keyCode, int scanCode, int action, int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			
			static int repeatCount = 0;

			if (action == GLFW_PRESS)
			{
				repeatCount = 1;
				KeyPressedEvent event(keyCode, repeatCount);
				data.EventCallback(event);
			}
			else if (action == GLFW_RELEASE)
			{
				repeatCount = 0;
				KeyReleasedEvent event(keyCode);
				data.EventCallback(event);
			}
			else if (action == GLFW_REPEAT)
			{
				repeatCount++;
				KeyPressedEvent event(keyCode, repeatCount);
				data.EventCallback(event);
			}
		});

		// Cursor callback
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			
			MouseMovedEvent event((float)xpos, (float)ypos);
			data.EventCallback(event);
		});

		// Mouse button callback
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			if (action == GLFW_PRESS)
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
			}
			else if (action == GLFW_RELEASE)
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			
			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});
	}

	void Window::OnEvent(Event& e)
	{
		// Dispatch the event to the application
		// Unhandled event error will be raised by the event handler
		if(m_Data.EventCallback)
			m_Data.EventCallback(e);
	}

	void Window::SetClearColor(float r, float g, float b, float a)
	{
		GX_PROFILE_FUNCTION()

		glClearColor(r, g, b, a);
	}

	void Window::SetVSync(bool enabled)
	{
		GX_PROFILE_FUNCTION()

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.Vsync = enabled;
	}

	bool Window::IsVSync() const
	{
		return m_Data.Vsync;
	}

	void Window::Clear()
	{
		GX_PROFILE_FUNCTION()

		/* Clear both color and depth buffer */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// More efficient to do in one function call
	}

	void Window::ClearDepthBuffer()
	{
		GX_PROFILE_FUNCTION()

		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void Window::OnUpdate()
	{
		GX_PROFILE_FUNCTION()

		/* Swap the front and back buffers */
		glfwSwapBuffers(m_Window);

		/* Poll for events */
		glfwPollEvents();
	}

	void Window::OnResize()
	{
		GX_PROFILE_FUNCTION()

		glViewport(0, 0, m_Data.Width, m_Data.Height);
	}

	void Window::SetCursorInputMode(CursorInputMode InputMode)
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)InputMode);
	}

	Window::~Window()
	{
		GX_ENGINE_INFO("Window: Destroying window");
		GX_PROFILE_FUNCTION()

		/* Destroy the Graphics Context */
		delete m_Context;

		/* Destroy the window */
		glfwDestroyWindow(m_Window);

		/* Terminate */
		glfwTerminate();
	}
}