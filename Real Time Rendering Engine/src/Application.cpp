#include <iostream>


#include "VertexArray.h"
#include "shaders/Shader.h"
#include "buffers/VertexBuffer.h"
#include "buffers/VertexBufferLayout.h"

#include "ErrorHandler.h"
#include "Window.h"

int main()
{
	using namespace engine;

	//Title of the window
	std::string title = "Real Time Rendering Engine";
	Window *window = new Window(title, 640, 480);

	// Initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!! Couldn't initialize glew." << std::endl;
		return -1;
	}

	// Print the gl version
	std::cout << glGetString(GL_VERSION) << std::endl;

	float vertices[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f,  0.5f
	};

	arrays::VertexArray vao;
	buffers::VertexBuffer vbo(vertices, 2 * 3 * sizeof(float));
	buffers::VertexBufferLayout layout;

	layout.Push<float>(2);

	vao.AddBuffer(vbo, layout);
	vao.UnBind();

	shaders::Shader shader("res/shaders/Basic.shader");
	shader.Bind();

	// Draw while the window doesn't close
	while (!window->IsClosed())
	{
		// Clear the window 
		window->Clear();

		vao.Bind();
		shader.Bind();
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

		//Draw the triangle - Legacy OpenGL
		/*glBegin(GL_TRIANGLES);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f( 0.5f, -0.5f);
		glVertex2f( 0.0f,  0.5f);
		glEnd();*/

		//Poll events and swap buffers
		window->Update();
	}

	//Delete the window
	delete window;

	std::cin.get();
	return 0;
}