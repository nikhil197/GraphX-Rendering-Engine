#include "Window.h"
#include <iostream>

namespace engine
{
	Window::Window(std::string title, int width, int height)
		: m_Title(title), m_Width(width), m_Height(height)
	{
		bool res = Init();
		if (!res)
		{
			std::cout << "Error while creating the window" << std::endl;
		}
	}

	bool Window::Init()
	{
		if (!glfwInit())
		{
			return false;
		}

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), NULL, NULL);

		if (!m_Window)
		{
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_Window);

		return true;
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

	void Window::Clear()
	{
		/* Clear the color buffer */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Clear the depth buffer */
		glClear(GL_DEPTH_BUFFER_BIT);
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
		/* Terminate the window */
		glfwTerminate();
	}

}