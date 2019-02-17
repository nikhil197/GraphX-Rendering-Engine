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

		/* Create a window showing the details of the cube */
		static void TransformWindow(std::string Name, gm::Vector3& translation, gm::Vector3& scale, float& rotation, const gm::Vector3& axis, bool& bShowMenu);

		/* Create a window showing the tranformation details of the mesh */
		static void TransformWindow(class Mesh3D& mesh);

		/* Create a window showing the details of the Mesh3D*/
		static void DetailsWindow(class Mesh3D& mesh);

		/* Create a window showing the light properties */
		static void LightProperties(class Light& light);

		/* Create a window showing the Camera properties */
		static void CameraProperties(class Camera& camera);

		/* Create a window showing the details of the Mesh2D*/
		static void DetailsWindow(class Mesh2D& mesh);

		/* Create a window for loading a model from directory */
		static void Models();

		/* Render the UI */
		static void Render();

		/* Cleanup the GUI at the close */
		static void Cleanup();
	};
}