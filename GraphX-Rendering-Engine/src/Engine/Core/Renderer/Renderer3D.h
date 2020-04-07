#pragma once

namespace GraphX
{
	class Shader;
	class Mesh3D;
	class Model3D;
	class Terrain;

	class Renderer3D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();
		
		/* Submit the object to be rendered */
		static void Submit(const Ref<Mesh3D>& mesh);

		/* Submit the model to be rendered */
		static void Submit(const Ref<Model3D>& model);

		/* Submit the terrain for rendering */
		static void Submit(const Ref<Terrain>& terrain);

		/* Renders the objects submitted to the rendered*/
		static void Render();

		/* Renders the objects submitted to the renderer to the depth framebuffer (Shader should be bound before calling the render method) */
		static void Render(Shader& DepthShader);

	private:
		/* Renders the collision bounds for debugging */
		static void RenderDebugCollisions(const Ref<GM::BoundingBox>& Box);

	private:
		struct Renderer3DData
		{
			/* Queue containing the objects to be rendered */
			std::deque<Ref<Mesh3D>> RenderQueue;

			struct Debug
			{
				Scope<class VertexArray> VAO;
				Scope<class VertexBuffer> VBO;
			} DebugData;

		};

		static Renderer3DData* s_Data;
	};
}