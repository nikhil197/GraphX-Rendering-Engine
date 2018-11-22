#include "GraphX_Maths.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "shaders/Shader.h"
#include "buffers/VertexBuffer.h"
#include "buffers/VertexBufferLayout.h"
#include "buffers/IndexBuffer.h"
#include "model/Vertex.h"

#include "timer/Clock.h"
#include "ErrorHandler.h"
#include "Window.h"
#include "Log.h"

int main()
{
	using namespace engine;
	using namespace gm;

	//Title of the window
	std::string title = "Real Time Rendering Engine";
	Window *window = new Window(title, 640, 480);

	Clock::Init();
	Log::Init();

	// Initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		GX_ENGINE_ERROR("Application: Error while initializing glew");
		return -1;
	}

	// Print the gl version
	std::cout << glGetString(GL_VERSION) << std::endl;

	// Vertices of the cube to be rendered
	Vertex vertices[] = {
		/*Vertex Positions*/	/* Normal Coordinates */	/* Colors */
		// Back face
		{ Vector3(-1, -1, -1),	  Vector3(-1, -1, -1),		Vector4(1.0f, 0.0f, 0.0f, 1.0f) },	//0
		{ Vector3( 1, -1, -1),	  Vector3( 1, -1, -1),		Vector4(0.0f, 1.0f, 0.0f, 1.0f) },	//1
		{ Vector3( 1,  1, -1),	  Vector3( 1,  1, -1),		Vector4(0.0f, 0.0f, 1.0f, 1.0f) },	//2
		{ Vector3(-1,  1, -1),	  Vector3(-1,  1, -1),		Vector4(1.0f, 1.0f, 0.0f, 1.0f) },	//3
								  
		// Front face			  
		{ Vector3(-1, -1,  1),	  Vector3(-1, -1,  1),		Vector4(0.0f, 1.0f, 1.0f, 1.0f) },	//4
		{ Vector3( 1, -1,  1),	  Vector3( 1, -1,  1),		Vector4(1.0f, 0.0f, 1.0f, 1.0f) },	//5
		{ Vector3( 1,  1,  1),	  Vector3( 1,  1,  1),		Vector4(1.0f, 0.5f, 1.0f, 1.0f) },	//6
		{ Vector3(-1,  1,  1),	  Vector3(-1,  1,  1),		Vector4(1.0f, 1.0f, 1.0f, 1.0f) }	//7
	};

	// Indices into the vertex buffer
	unsigned int indices[] = {
		// Back face
		0, 1, 2,
		0, 2, 3,

		// Top Face
		7, 6, 2,
		7, 2, 3,

		// Front Face
		4, 5, 6,
		4, 6, 7,

		// Left Face
		0, 4, 7,
		0, 7, 3,

		// Right face
		5, 1, 2,
		5, 2, 6,

		// Bottom Face
		4, 5, 1,
		4, 5, 0
	};

	VertexArray vao;
	VertexBuffer vbo(vertices, 8 * sizeof(Vertex));
	VertexBufferLayout layout;
	IndexBuffer ibo(indices, 36);
	
	// Layout for the vertex positions
	layout.Push<float>(Vector3::Components);

	// Layout for the vertex normals
	layout.Push<float>(Vector3::Components);

	// Layout for the vertex colors
	layout.Push<float>(Vector4::Components);

	vao.AddBuffer(vbo, layout);
	ibo.UnBind();
	vao.UnBind();

	Shader shader("res/shaders/BasicLightingShader.shader");
	shader.Bind();

	Renderer renderer;

	// Model Matrix
	Translation trans(Vector3(0));
	Rotation rotate(Vector3(0));
	Scaling scale(Vector3(1));
	Matrix4 model = trans * rotate * scale;
	shader.SetUniformMat4f("u_Model", model);

	// View Matrix
	Matrix4 view = View::LookAt(Vector3(2, 5, 3), Vector3(0, 0, 0), Vector3::YAxis);
	shader.SetUniformMat4f("u_View", view);

	// Projection Matrix
	Matrix4 proj = Projection::Ortho(-6.0f, 6.0f, -4.5f, 4.5f, 0.1f, -10.0f);
	shader.SetUniformMat4f("u_Projection", proj);

	float r = 0.0f;
	float increment = 0.01f;

	int times = 0;
	float then = Clock::GetClock()->GetTime();

	// Draw while the window doesn't close
	while (!window->IsClosed())
	{
		// Tick the clock every frame to get the delta time
		Clock::GetClock()->Tick();

		times++;
		float now = Clock::GetClock()->GetTime();
		if ((now - then) > 1.0f)
		{
			GX_ENGINE_INFO("Frame Rate: {0} FPS", times);
			then = now;
			times = 0;
		}

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