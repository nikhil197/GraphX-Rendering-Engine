#pragma once

namespace GraphX
{
	class Mesh2D;
	class Shader;
	class Texture2D;

	class Renderer2D
	{
		friend class Batch2D;
		friend class ParticleBatch;
	public:
		/* Renderer2D Statistics */
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetVertexCount() const { return QuadCount * 4; }
			uint32_t GetIndexCount() const { return QuadCount * 6; }
		};

	public:
		static void Init();
		static void Shutdown();
			   
		static void BeginScene();
		static void EndScene();

		static void DrawQuad(const GM::Vector2& position, const GM::Vector2& size, const GM::Vector4& color);
		static void DrawQuad(const GM::Vector3& position, const GM::Vector2& size, const GM::Vector4& color);

		static void DrawQuad(const GM::Vector2& position, const GM::Vector2& size, const Ref<Texture2D>& texture, const GM::Vector4& tintColor = GM::Vector4::UnitVector, float tiling = 1.0f, uint32_t textureSlot = 0);
		static void DrawQuad(const GM::Vector3& position, const GM::Vector2& size, const Ref<Texture2D>& texture, const GM::Vector4& tintColor = GM::Vector4::UnitVector, float tiling = 1.0f, uint32_t textureSlot = 0);

		static void DrawRotatedQuad(const GM::Vector2& position, const GM::Vector2& size, const GM::Vector3& rotation, const GM::Vector4& color);
		static void DrawRotatedQuad(const GM::Vector3& position, const GM::Vector2& size, const GM::Vector3& rotation, const GM::Vector4& color);

		static void DrawRotatedQuad(const GM::Vector2& position, const GM::Vector2& size, const GM::Vector3& rotation, const Ref<Texture2D>& texture, const GM::Vector4& tintColor = GM::Vector4::UnitVector, float tiling = 1.0f, uint32_t textureSlot = 0);
		static void DrawRotatedQuad(const GM::Vector3& position, const GM::Vector2& size, const GM::Vector3& rotation, const Ref<Texture2D>& texture, const GM::Vector4& tintColor = GM::Vector4::UnitVector, float tiling = 1.0f, uint32_t textureSlot = 0);

		// Only for debug Purpose
		static void DrawDebugQuad(const GM::Vector3& position, const GM::Vector2& size, const Ref<Texture2D>& texture, uint32_t textureSlot);

		/* Renders the active particles from the pool of particles */
		static void RenderParticles(const std::unordered_map<std::string, Ref<class ParticleSystem>>& ParticleSystems);

		/* Submit the mesh to be rendered to the render */
		static void Submit(const Ref<Mesh2D>& mesh);

		/* Renders the objects submitted to the renderer */
		static void Render();

		/* Renders the objects submitted to the renderer to the depth framebuffer (Shader should be bound before calling the render method) */
		static void Render(Shader& DepthShader);

		/* Resets the stats back to 0 */
		static void ResetStats();

		/* Returns the renderer stats */
		static const Renderer2D::Statistics& GetStats() { return s_Data->Stats; }

	private:
		/* Internal method to draw quad - Reduce code repetition */
		static void DrawQuad_Internal(const GM::Matrix4& transform, const GM::Vector4& color);
		static void DrawQuad_Internal(const Ref<Texture2D>& texture, const GM::Matrix4& transform, const GM::Vector4& color, float tiling, uint32_t textureSlot);

		/* Internal method to render particles in batches */
		static void RenderParticlesBatched_Internal(const std::unordered_map<std::string, Ref<ParticleSystem>>& ParticleSystems);
	private:
		struct Renderer2DData
		{
			// Vertex Array to store the quad vertices and indices
			Scope<class VertexArray> QuadVA;

			// One Shader for rendering all 2D stuff
			Ref<Shader> TextureShader;

			// Debug shader for rendering depth map (TODO: Find a better place for this)
			Ref<Shader> ShadowDebugShader;

			// White texture for rendering colored 2D stuff
			Scope<Texture2D> WhiteTexture;

			// Batch object for batching quads / meshes together
			Scope<class Batch2D> Batch;

			// Shader used to render the batch
			Ref<class Shader> BatchShader;

			// Batch object for batching particles together
			Scope<class ParticleBatch> ParticleBatch;

			// Shader to render particles (without batch)
			Ref<class Shader> ParticleShader;

			// Shader to render the particle batch
			Ref<class Shader> ParticleBatchShader;

			/* Queue containing the objects to be rendered */
			std::deque<Ref<Mesh2D>> RenderQueue;
			
			/* Renderer Draw statistics */
			Renderer2D::Statistics Stats;
		};

		static Renderer2DData* s_Data;
	};
}