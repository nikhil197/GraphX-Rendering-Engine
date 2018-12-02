#include "pch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "shaders/Shader.h"
#include "buffers/VertexBuffer.h"
#include "buffers/VertexBufferLayout.h"
#include "buffers/IndexBuffer.h"
#include "model/Vertex.h"
#include "entities/Light.h"

#include "timer/Clock.h"
#include "Window.h"
#include "gui/GraphXGui.h"

int main()
{
	using namespace engine;
	using namespace gm;

	// Initialise the clock and the logging
	Log::Init();
	Clock::Init();
	
	//Title of the window
	std::string title = "Real Time Rendering Engine";
	Window *window = new Window(title, 640, 480);

	// Initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		GX_ENGINE_ERROR("Application: Error while initializing glew");
		return -1;
	}

	// Print the gl version
	GX_ENGINE_INFO(glGetString(GL_VERSION));

	// To enable the depth test
	GLCall(glEnable(GL_DEPTH_TEST));

	// Vertices of the cube to be rendered
	Vertex vertices[] = {
		/*Vertex Positions*/	/* Normal Coordinates */	/* Colors */
		// Front face
		{ Vector3(-1, -1,  1),	  Vector3(-1, -1,  1),		Vector4(1.0f, 0.0f, 0.0f, 1.0f) },	//0
		{ Vector3( 1, -1,  1),	  Vector3( 1, -1,  1),		Vector4(1.0f, 0.0f, 0.0f, 1.0f) },	//1
		{ Vector3( 1,  1,  1),	  Vector3( 1,  1,  1),		Vector4(1.0f, 0.0f, 0.0f, 1.0f) },	//2
		{ Vector3(-1,  1,  1),	  Vector3(-1,  1,  1),		Vector4(1.0f, 0.0f, 0.0f, 1.0f) },	//3
								  
		// Back face			  
		{ Vector3(-1, -1, -1),	  Vector3(-1, -1, -1),		Vector4(0.0f, 1.0f, 0.0f, 1.0f) },	//4
		{ Vector3( 1, -1, -1),	  Vector3( 1, -1, -1),		Vector4(0.0f, 1.0f, 0.0f, 1.0f) },	//5
		{ Vector3( 1,  1, -1),	  Vector3( 1,  1, -1),		Vector4(0.0f, 1.0f, 0.0f, 1.0f) },	//6
		{ Vector3(-1,  1, -1),	  Vector3(-1,  1, -1),		Vector4(0.0f, 1.0f, 0.0f, 1.0f) }	//7
	};

	// Indices into the vertex buffer
	unsigned int indices[] = {
		// Front face
		3, 0, 2,
		2, 0, 1,

		// Top Face
		7, 3, 6,
		6, 3, 2,

		// Back Face
		6, 5, 7,
		7, 5, 4,

		// Bottom Face
		0, 4, 1,
		1, 4, 5,

		// Right face
		2, 1, 6,
		6, 1, 5,

		// Left Face
		7, 4, 3,
		3, 4, 0
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

	// Add the layout to the vertex array
	vao.AddBuffer(vbo, layout);
	ibo.UnBind();
	vao.UnBind();

	// Basic Lighting Shader 
	Shader shader("res/shaders/BasicLightingShader.shader");
	shader.Bind();

	// Simple Renderer to render the objects
	Renderer renderer;

	// View Matrix
	Vector3 CameraPos(0, 0, 3.0f);
	Matrix4 view = View::LookAt(CameraPos, Vector3(0, 0, 0), Vector3::YAxis);
	shader.SetUniformMat4f("u_View", view);

	// Projection Matrix
	Matrix4 proj = Projection::Ortho(-6.0f, 6.0f, -4.5f, 4.5f, -10.0f, 10.0f);
	shader.SetUniformMat4f("u_Projection", proj);

	Light light(Vector3(0, 0, 20.0f), Vector4(1, 1, 1, 1));
	shader.SetUniform3f("u_LightPos", light.Position);
	shader.SetUniform4f("u_LightColor", light.Color);

	int times = 0;
	float then = Clock::GetClock()->GetTime();

	float rotation = 0.0f;
	Vector3 translation(0);
	Vector3 scaleVec(1);
	Vector3 axis(1, 0, 0);

	bool bShowMenu = true;

	// Draw while the window doesn't close
	while (!window->IsClosed())
	{
		// Tick the clock every frame to get the delta time
		Clock::GetClock()->Tick();

		// Update the Gui
		GraphXGui::Update();

		// Calculate the fps
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

		// Bind the shader and draw the objects
		shader.Bind();

		// Get a new transform window for the cube
		GraphXGui::TransformWindow("Transform", translation, scaleVec, rotation, axis, bShowMenu);

		// Render the GUI
		GraphXGui::Render();

		// Model Matrix
		Translation trans(translation);
		Rotation rotate(rotation * Clock::GetClock()->GetTime(), axis);
		Scaling scale(scaleVec);
		Matrix4 model = trans * rotate * scale;
		shader.SetUniformMat4f("u_Model", model);

		// Normal Transform Matrix (Could be done in the vertex shader, but more efficient here since vertex shader runs for each vertex)
		Matrix3 normal = Matrix3(view * model);
		shader.SetUniformMat3f("u_Normal", normal);

		// Render the Cube
		renderer.Draw(vao, ibo, shader);
		
		//Poll events and swap buffers
		window->Update();
	}

	//Delete the window
	delete window;

	std::cin.get();
	return 0;
}