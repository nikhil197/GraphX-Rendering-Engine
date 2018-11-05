#include "Renderer.h"
#include "ErrorHandler.h"

#include "buffers/IndexBuffer.h"
#include "VertexArray.h"
#include "shaders/Shader.h"

namespace engine
{
	void Renderer::Draw(const VertexArray& vao, const Shader& shader, int count) const
	{
		vao.Bind();
		glDrawArrays(GL_TRIANGLES, 0, count);
	}

	void Renderer::Draw(const VertexArray& vao, const IndexBuffer& ibo, const Shader& shader) const
	{
		vao.Bind();
		ibo.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
	}
}