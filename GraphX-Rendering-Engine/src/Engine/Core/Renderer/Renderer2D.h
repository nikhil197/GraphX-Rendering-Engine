#pragma once

namespace GraphX
{
	class Mesh2D;
	class Shader;
	class Texture2D;

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
			   
		static void BeginScene();
		static void EndScene();

		static void DrawQuad(const GM::Vector2& position, const GM::Vector2& size, const GM::Vector4& color);
		static void DrawQuad(const GM::Vector3& position, const GM::Vector2& size, const GM::Vector4& color);

		static void DrawQuad(const GM::Vector2& position, const GM::Vector2& size, const Ref<Texture2D>& texture, unsigned int slot = 0);
		static void DrawQuad(const GM::Vector3& position, const GM::Vector2& size, const Ref<Texture2D>& texture, unsigned int slot = 0);

		// Only for debug Purpose
		static void DrawDebugQuad(const GM::Vector3& position, const GM::Vector2& size, const Ref<Texture2D>& texture, unsigned int slot);

		/* Renders the active particles from the pool of particles */
		static void RenderParticles(const std::vector<class Particle>& particles);

		/* Submit the mesh to be rendered to the render */
		static void Submit(const Ref<Mesh2D>& mesh);

		/* Renders the objects submitted to the renderer */
		static void Render();

		/* Renders the objects submitted to the renderer to the depth framebuffer (Shader should be bound before calling the render method) */
		static void Render(Shader& DepthShader);

	private:
		struct Renderer2DStorage
		{
			// Vertex Array to store the quad vertices and indices
			Scope<class VertexArray> QuadVA;

			// One Shader for rendering all 2D stuff
			Ref<Shader> TextureShader;

			// Debug shader for rendering depth map (TODO: Find a better place for this)
			Ref<Shader> ShadowDebugShader;

			// White texture for rendering colored 2D stuff
			Scope<Texture2D> WhiteTexture;

			/* Queue containing the objects to be rendered */
			std::deque<Ref<Mesh2D>> RenderQueue;
		};

		static Renderer2DStorage* s_Data;
	};
}