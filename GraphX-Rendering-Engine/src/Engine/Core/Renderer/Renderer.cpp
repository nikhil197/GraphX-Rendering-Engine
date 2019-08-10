#include "pch.h"
#include "Renderer.h"

#include "Renderer3D.h"
#include "Renderer2D.h"
#include "SimpleRenderer.h"

#include "Model\Mesh\Mesh2D.h"
#include "Model\Mesh\Mesh3D.h"

#include "Shaders\Shader.h"

namespace GraphX
{
	Renderer3D* Renderer::s_Renderer3D = nullptr;
	Renderer2D* Renderer::s_Renderer2D = nullptr;
	SimpleRenderer* Renderer::s_Renderer = nullptr;

	void Renderer::Initialize()
	{
		s_Renderer = new SimpleRenderer();
		s_Renderer2D = new Renderer2D();
		s_Renderer3D = new Renderer3D();
	}

	void Renderer::CleanUp()
	{
		delete s_Renderer;
		delete s_Renderer2D;
		delete s_Renderer3D;
	}

	void Renderer::BeginScene()
	{

	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const class Mesh2D* Mesh)
	{
		s_Renderer2D->Submit(Mesh);
	}

	void Renderer::Submit(const class Mesh3D* Mesh)
	{
		s_Renderer3D->Submit(Mesh);
	}

	void Renderer::Submit(const class Model3D* Model)
	{
		s_Renderer3D->Submit(Model);
	}

	void Renderer::Submit(const class Terrain* Terr)
	{
		s_Renderer3D->Submit(Terr);
	}

	void Renderer::Render()
	{
		s_Renderer3D->Render();
		s_Renderer2D->Render();
	}

	void Renderer::RenderDepth(Shader& DepthShader)
	{
		s_Renderer3D->Render(DepthShader);
		s_Renderer2D->Render(DepthShader);
	}

	void Renderer::Render(unsigned int Count)
	{
		s_Renderer->Draw(Count);
	}

	void Renderer::RenderIndexed(const IndexBuffer& indexBuffer)
	{
		s_Renderer->DrawIndexed(indexBuffer);
	}
}