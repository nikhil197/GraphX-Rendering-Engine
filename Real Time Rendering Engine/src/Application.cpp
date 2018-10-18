#include <iostream>

#include "Window.h"

int main()
{
	std::string title = "Real Time Rendering Engine";
	engine::Window window(title, 640, 480);

	while (!window.IsClosed())
	{
		// Clear the window 
		window.Clear();

		//Draw the triangle - Legacy OpenGL
		glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f( 0.5f, -0.5f);
		glVertex2f( 0.0f,  0.5f);
		glEnd();

		//Poll events and swap buffers
		window.Update();
	}

	std::cin.get();
	return 0;
}