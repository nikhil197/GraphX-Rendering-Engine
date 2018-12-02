#pragma once

struct GLFWwindow;

namespace engine
{
	// This will be a static class to create ui elements and will serve as an abstraction for the ImGui
	class GraphXGui
	{
	private:
		GraphXGui();

	public:
		/* Initialize the GUI */
		static void Init(GLFWwindow* window, bool bSetupCallbacks = false);

		/* Update the UI frame */
		static void Update();

		static void TransformWindow(std::string Name, gm::Vector3& translation, gm::Vector3& scale, float& rotation, const gm::Vector3& axis, bool& bShowMenu);

		/* Render the UI */
		static void Render();

		/* Cleanup the GUI at the close */
		static void Cleanup();
	};
}