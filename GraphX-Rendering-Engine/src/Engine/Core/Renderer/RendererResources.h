#pragma once

namespace GraphX
{
	/**
	 * Data required for rendering the skybox
	 */
	struct SkyboxRenderData
	{
		/* Vertex Array for the skybox */
		Scope<class VertexArray> VAO;

		/* Vertex Buffer for the skybox */
		Scope<class VertexBuffer> VBO;

		/* Index Buffer for the skybox */
		Ref<class IndexBuffer> IBO;

		/* Shader used for the skybox */
		Ref<class Shader> SkyboxShader;
	};
}