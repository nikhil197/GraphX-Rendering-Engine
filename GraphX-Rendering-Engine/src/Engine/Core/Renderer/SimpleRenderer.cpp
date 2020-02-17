#include "pch.h"
#include "SimpleRenderer.h"
#include "Gl/glew.h"

#include "Buffers/IndexBuffer.h"

namespace GraphX
{
	void SimpleRenderer::Draw(int count) const
	{
		GX_PROFILE_FUNCTION()

		glDrawArrays(GL_TRIANGLES, 0, count);
	}

	void SimpleRenderer::DrawIndexed(const IndexBuffer& ibo) const
	{
		GX_PROFILE_FUNCTION()

		glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}