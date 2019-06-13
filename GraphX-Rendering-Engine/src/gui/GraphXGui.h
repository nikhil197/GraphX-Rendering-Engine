#pragma once

struct GLFWwindow;

namespace engine
{
	// This will be a static class to create ui elements and will serve as an abstraction for the ImGui
	class GraphXGui
	{
	private:
		GraphXGui();

		/* Callback*/
		static std::function<void(class Event&)> s_GuiEventCallback;

	public:
		/* Initialize the GUI */
		static void Init(GLFWwindow* window, const std::function<void(class Event&)>& callback, bool bSetupCallbacks = false);

		/* Update the UI frame */
		static void Update();

		/* Create a window showing Global Settings like Day Time and Skybox */
		static void GlobalSettings(class Skybox& skybox, float& daytime, float& SunlightIntensity, bool& EnableParticles);

		/* Create a window showing Add Terrain Button */
		static void AddTerrain();

		/* Create a window showing the details of the Mesh3D*/
		static void DetailsWindow(class Mesh3D& mesh, const std::string& Name = "Details");

		/* Create a window showing the light properties */
		static void LightProperties(class PointLight& light);

		/* Create a window showing the Camera properties */
		static void CameraProperties(class Camera& camera);

		/* Create a window showing the details of the Mesh2D */
		static void DetailsWindow(class Mesh2D& mesh);

		/* Create a window showing the details of the Terrain */
		static void TerrainDetails(class Terrain& terrain);

		/* Create a window for loading a model from directory */
		static void Models();

		/* Render the UI */
		static void Render();

		/* Cleanup the GUI at the close */
		static void Cleanup();
	};
}