#include "pch.h"
#include "GraphXGui.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"

namespace engine
{
	void GraphXGui::Init(GLFWwindow* window, bool bSetupCallbacks)
	{
		Timer timer("Initialising ImGui");

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, bSetupCallbacks);

		ImGui::StyleColorsDark();
	}

	void GraphXGui::Update()
	{
		ImGui_ImplGlfwGL3_NewFrame();
	}

	void GraphXGui::TransformWindow(std::string Name, gm::Vector3& translation, gm::Vector3& scale, float& rotation, const gm::Vector3& axis, bool& bShowMenu)
	{
		if (bShowMenu)
		{
			ImGui::Begin(Name.c_str(), (bool*)bShowMenu);
			ImGui::SliderFloat3("Translation", (float*)(&translation.x), -10.0f, 10.0f);
			ImGui::SliderFloat("Rotation", (float*)(&rotation), 0.0f, 359.f);
			ImGui::SliderFloat3("Rotation Axis", (float*)(&axis.x), 0.0f, 1.0f);
			ImGui::SliderFloat3("Scale", (float*)(&scale.x), 0.00000001f, 10.f);

			if (ImGui::Button("Close"))
			{
				bShowMenu = false;
			}

			ImGui::End();
		}
	}

	void GraphXGui::Render()
	{
		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GraphXGui::Cleanup()
	{
		GX_ENGINE_INFO("GraphXGui: Cleaning up ImGui");
		ImGui_ImplGlfwGL3_Shutdown();
		ImGui::DestroyContext();
	}
}