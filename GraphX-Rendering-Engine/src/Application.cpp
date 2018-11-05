#include <iostream>

#include "vectors/Vector2.h"
#include "vectors/Vector3.h"
#include "matrices/Matrix4.h"
#include "transformations/Translation.h"

#include "VertexArray.h"
#include "Renderer.h"
#include "shaders/Shader.h"
#include "buffers/VertexBuffer.h"
#include "buffers/VertexBufferLayout.h"
#include "buffers/IndexBuffer.h"

#include "ErrorHandler.h"
#include "Window.h"

int main()
{
	using namespace engine;
	using namespace gm;

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

	Vector2 vertices[] = {
		{-0.5f, -0.5f}, //0
		{ 0.5f, -0.5f}, //1
		{ 0.5f,  0.5f}, //2
		{-0.5f,  0.5f}  //3
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	VertexArray vao;
	VertexBuffer vbo(vertices, 4 * sizeof(Vector2));
	VertexBufferLayout layout;
	IndexBuffer ibo(indices, 6);
	layout.Push<float>(Vector2::Components);

	vao.AddBuffer(vbo, layout);
	ibo.UnBind();
	vao.UnBind();

	Shader shader("res/shaders/Basic.shader");
	shader.Bind();

	Renderer renderer;

	float r = 0.0f;
	float increment = 0.01f;

	// Draw while the window doesn't close
	while (!window->IsClosed())
	{
		// Clear the window 
		window->Clear();

		// Resize the window before rendering
		window->Resize();

		shader.Bind();
		shader.SetUniform4f("u_Color", r, 1.0f, r, 1.0);
		renderer.Draw(vao, ibo, shader);

		if (r > 1.0f)
		{
			increment = -0.05f;
		}
		else if (r < 0.0f)
		{
			increment = 0.05f;
		}
		r += increment;

		//Poll events and swap buffers
		window->Update();
	}

	//Delete the window
	delete window;

	std::cin.get();
	return 0;
}