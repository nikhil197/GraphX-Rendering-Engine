#pragma once

#include "Core/Renderer/RendererTypes.h"

namespace GraphX
{
	class Shader;
	class Mesh3D;
	class Terrain;

	class Renderer3D
	{
		friend class Renderer;
	public:
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t GeometryCount = 0;
			uint32_t IndexCount = 0;
			uint32_t CollisionBoxes = 0;
		};
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();
		
		/* Submit the object to be rendered */
		static void Submit(const Ref<Mesh3D>& mesh);

		/* Submit the terrain for rendering */
		static void Submit(const Ref<Terrain>& terrain);

		/* Renders the objects submitted to the rendered*/
		static void Render();

		/* Renders the objects submitted to the renderer to the depth framebuffer (Shader should be bound before calling the render method) */
		static void Render(Shader& DepthShader);

		static const Renderer3D::Statistics& GetStats() { return s_Data->Stats; }

		static void ResetStats();

		/* The actual Draw call for the GPU to render geometry */
		static void DrawCall(uint32_t RenderMode, uint32_t Count, uint32_t Type, const void* indices = nullptr);

		/* The actual Draw call for the GPU to render instanced geometry */
		static void DrawCallInstanced(uint32_t RenderMode, uint32_t IndicesCount, uint32_t Type, uint32_t InstanceCount);

	private:
		static void RenderNormal();
		
		/* Renders the collision bounds for debugging */
		static void RenderDebugCollisions(const Ref<GM::BoundingBox>& Box);

		/* Renders the submitted objects using instanced rendering */
		static void RenderInstanced();

		static void OnRenderModeChange(RenderMode newMode);

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

			Statistics Stats;
		};

		static Renderer3DData* s_Data;

		static std::unordered_map<std::size_t, Ref<class InstanceBatch>> s_InstanceBatches;
	};
}