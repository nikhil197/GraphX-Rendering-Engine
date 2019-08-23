#include "pch.h"
#include "SimpleRenderer.h"
#include "ErrorHandler.h"

#include "Buffers/IndexBuffer.h"

namespace GraphX
{
	void SimpleRenderer::Draw(int count) const
	{
		GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
	}

	void SimpleRenderer::DrawIndexed(const IndexBuffer& ibo) const
	{
		GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
	}
}