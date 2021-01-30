#include "pch.h"
#include "Quad.h"

#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"
#include "VertexArray.h"

namespace GraphX
{
	const GM::Vector3 Quad::s_QuadVertexPositions[4] = {
		{ -0.5f, -0.5f, 0.0f },
		{  0.5f, -0.5f, 0.0f },
		{  0.5f,  0.5f, 0.0f },
		{ -0.5f,  0.5f, 0.0f }
	};

	const GM::Vector2 Quad::s_QuadVertexTexCoords[4] = {
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f }
	};

	const uint32_t Quad::s_QuadIndices[6] = { 0, 1, 2, 2, 3, 0 };

	Quad::Quad()
	{
		VertexBuffer vbo(&s_QuadVertexPositions[0], s_QuadVertexCount * sizeof(GM::Vector2));
		VertexBufferLayout layout = {
			{ BufferDataType::Float2 }
		};

		m_VAO = CreateScope<VertexArray>();
		m_VAO->AddVertexBuffer(vbo, layout);
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
	}
}