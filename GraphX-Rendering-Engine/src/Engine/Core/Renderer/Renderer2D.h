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
			   
		static void BeginScene(const Ref<class Camera>& MainCamera);
		static void EndScene();

		static void DrawQuad(const GM::Vector2& position, const GM::Vector2& size, const GM::Vector4& color);
		static void DrawQuad(const GM::Vector3& position, const GM::Vector2& size, const GM::Vector4& color);

		static void DrawQuad(const GM::Vector2& position, const GM::Vector2& size, const Ref<Texture2D>& texture);
		static void DrawQuad(const GM::Vector3& position, const GM::Vector2& size, const Ref<Texture2D>& texture);

		/* Submit the mesh to be rendered to the render */
		static void Submit(const Ref<Mesh2D>& mesh);

		/* Renders the objects submitted to the renderer */
		static void Render();

		/* Renders the objects submitted to the renderer to the depth framebuffer (Shader should be bound before calling the render method) */
		static void Render(Shader& DepthShader);

	private:
		struct Renderer2DStorage
		{
			Ref<class Camera> SceneCamera;

			// One Shader for rendering all 2D stuff
			Ref<Shader> TextureShader;

			// White texture for rendering colored 2D stuff
			Ref<Texture2D> WhiteTexture;

			/* Queue containing the objects to be rendered */
			std::deque<Ref<Mesh2D>> RenderQueue;
		};

		static Renderer2DStorage* s_Data;
	};
}