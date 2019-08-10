#pragma once

namespace GraphX
{
	class Renderer
	{
	public:
		/* Initializes the renderer */
		static void Initialize();

		/* Cleans up at application close */
		static void CleanUp();

		/* Begin a Scene for rendering */
		static void BeginScene();

		/* Marks the end of a scene */
		static void EndScene();

		/* Submit a 2D mesh for rendering */
		static void Submit(const class Mesh2D* mesh);

		/* Submit a 3D mesh for rendering */
		static void Submit(const class Mesh3D* mesh);

		/* Submit a 3D model for rendering */
		static void Submit(const class Model3D* model);

		/* Submit a terrain for rendering */
		static void Submit(const class Terrain* terrain);

		/* Renders all the submitted meshes in the scene */
		static void Render();

		/* Renders all the submitted meshes in the scene to the depth buffer */
		static void RenderDepth(class Shader& DepthShader);

		/* Directly renders (without queueing) the current bound vertex array having Count number of vertices
		* NOTE: This is not for indexed rendering
		*/
		static void Render(unsigned int Count);

		/* Directly renders (without queuing) the current bound vertex array indexed by 'indexBuffer' */
		static void RenderIndexed(const class IndexBuffer& indexBuffer);

	private:
		struct SceneInfo
		{

		};

		/* 3D Renderer for rendering 3D objects */
		static class Renderer3D* s_Renderer3D;

		/* 2D Renderer for rendering 2D objects */
		static class Renderer2D* s_Renderer2D;

		/* A Simple renderer to directly render stuff (without queuing) */
		static class SimpleRenderer* s_Renderer;
	};
}