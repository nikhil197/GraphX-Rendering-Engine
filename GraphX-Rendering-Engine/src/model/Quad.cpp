#include "pch.h"
#include "Quad.h"

#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "VertexArray.h"

namespace GraphX
{
	std::vector<GraphXMaths::Vector2> Quad::s_Vertices = 
	{
		GraphXMaths::Vector2(-0.5f, -0.5f),
		GraphXMaths::Vector2( 0.5f, -0.5f),
		GraphXMaths::Vector2( 0.5f,  0.5f),
		GraphXMaths::Vector2( 0.5f,  0.5f),
		GraphXMaths::Vector2(-0.5f,  0.5f),
		GraphXMaths::Vector2(-0.5f, -0.5f)
	};

	const unsigned int Quad::s_QuadVertexCount = 6;

	Quad::Quad()
	{
		VertexBuffer vbo(&s_Vertices[0], s_Vertices.size() * sizeof(GraphXMaths::Vector2));
		VertexBufferLayout layout;
		layout.Push<float>(GraphXMaths::Vector2::Components);

		m_VAO = new VertexArray();
		m_VAO->AddBuffer(vbo, layout);
		m_VAO->UnBind();
	}

	void Quad::Enable() const
	{
		m_VAO->Bind();
	}

	void Quad::Disable() const
	{
		m_VAO->UnBind();
	}

	Quad::~Quad()
	{
		delete m_VAO;
	}
}