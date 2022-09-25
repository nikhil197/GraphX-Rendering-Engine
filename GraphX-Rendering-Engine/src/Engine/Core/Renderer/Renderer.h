#pragma once

#include "Engine/Core/Shaders/ShaderLibrary.h"
#include "Engine/Core/Textures/TextureLibrary.h"
#include "RendererResources.h"

#include "Subsystems/Multithreading/Base/RunnableThread.h"

namespace GraphX
{
	class Mesh2D;
	class Mesh3D;
	class Shader;
	class IndexBuffer;

	class Camera;
	class Terrain;

	class SimpleRenderer;
	class Renderer2D;
	class Renderer3D;

	/* Returns if the current thread is the rendering thread */
	extern bool IsInRenderingThread(); // To be defined in the Thread Manager

	class Renderer
	{
		friend class Renderer2D;
		friend class Renderer3D;
		friend class ParticleBatch;
	public:
		/* Initializes the renderer */
		static void Init();

		/* Cleans up at application close */
		static void Shutdown();

		/* Begin a Scene for rendering */
		static void BeginScene(const Ref<Camera>& MainCamera);

		/* Marks the end of a scene */
		static void EndScene();

		/* Submit a 2D mesh for rendering */
		static void Submit(const Ref<Mesh2D>& mesh);

		/* Submit a 3D mesh for rendering */
		static void Submit(const Ref<Mesh3D>& mesh);

		/* Submit a terrain for rendering */
		static void Submit(const Ref<Terrain>& terrain);

		static void SubmitRenderCommand(class RenderCommandBase* Cmd);

		/* Render the skybox */
		static void RenderSkybox(const Ref<class Skybox>& skybox);

		/* Renders all the submitted meshes in the scene */
		static void Render();

		/* Renders all the submitted meshes in the scene using instanced rendering */
		static void RenderInstanced();

		/* Renders all the submitted meshes in the scene to the depth buffer */
		static void RenderDepth(class Shader& DepthShader);

		/* Directly renders (without queuing) the current bound vertex array having Count number of vertices
		* NOTE: This is not for indexed rendering
		*/
		static void Render(unsigned int Count);

		/* Directly renders (without queuing) the current bound vertex array indexed by 'indexBuffer' */
		static void RenderIndexed(const IndexBuffer& indexBuffer);

		/* Returns the shader used for debug rendering */
		static Ref<Shader> GetDebugCollisionsShader() { return s_DebugShader; }

		/* Returns the shader library */
		static ShaderLibrary& GetShaderLibrary() { return s_ShaderLibrary; }

		/* Returns the texture library */
		static TextureLibrary& GetTextureLibrary() { return s_TextureLibrary; }

		/* Returns the thread Id for the render thread */
		static std::thread::id GetRenderThreadID() { return s_RenderThread->GetID(); }

	public:
		// Maximum number of texture slots available for the renderer 
		static constexpr uint32_t MaxTextureImageUnits = 32;	// TODO: Move this to a more appropriate location

	private:
		struct SceneInfo
		{
			/* Main Camera of the scene */
			Ref<Camera> SceneCamera;

			/* Resets the scene info */
			void Reset() {}
		};

		/* Shaders library to store all the shaders */
		static ShaderLibrary s_ShaderLibrary;

		/* Textures library to store all the textures */
		static TextureLibrary s_TextureLibrary;

		static Ref<Shader> s_DebugShader;

		/* Info required for rendering current scene */
		static SceneInfo* s_SceneInfo;
		
		/* Renderer data required for the skybox */
		static SkyboxRenderData* s_SkyboxData;

		/* A Simple renderer to directly render stuff (without queuing) */
		static SimpleRenderer* s_Renderer;

		/* Render thread */
		static RunnableThread* s_RenderThread;

		static class RenderThread* s_RenderThreadRunnable;
	};
}