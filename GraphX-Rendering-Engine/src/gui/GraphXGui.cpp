#include "pch.h"
#include "GraphXGui.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"

#include "model/mesh/Mesh3D.h"
#include "model/mesh/Mesh2D.h"

#include "Textures/Texture.h"

#include "entities/Lights/Light.h"
#include "entities/Camera.h"

#include "Input/Mouse.h"

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

	void GraphXGui::TransformWindow(Mesh3D& mesh)
	{
		int x = sizeof(mesh);
		if (mesh.bShowDetails)
		{
			ImGui::Begin("Transformations", (bool*)mesh.bShowDetails);
			ImGui::SliderFloat3("Translation", (float*)&mesh.Position.x, -2000.0f, 2000.0f);
			ImGui::SliderFloat3("Rotation", (float*)(&mesh.Rotation), 0.0f, 359.f);
			ImGui::SliderFloat3("Scale", (float*)(&mesh.Scale.x), 0.0001f, 10.f);
			ImGui::End();
		}		
	}

	void GraphXGui::DetailsWindow(Mesh3D& mesh)
	{
		if (mesh.bShowDetails)
		{
			ImGui::Begin("Details", (bool*)mesh.bShowDetails);
			ImGui::Spacing();
			ImGui::Text("Transformations");
			ImGui::SliderFloat3("Translation", (float*)&mesh.Position.x, -1000.0f, 1000.0f);
			ImGui::SliderFloat3("Rotation", (float*)(&mesh.Rotation), 0.0f, 359.f);
			ImGui::SliderFloat3("Scale", (float*)(&mesh.Scale.x), 0.0001f, 10.f);
			ImGui::Spacing();
			ImGui::Text("Color and Properties");
			ImGui::SliderFloat("Shininess", (float*)&mesh.Shininess, 2.0f, 256.0f);
			ImGui::SliderFloat("Reflectivity", (float*)&mesh.Reflectivity, 0.0f, 1.0f);
			ImGui::ColorEdit4("Base Color", (float*)&mesh.BaseColor);
			ImGui::Spacing();

			ImGui::Text("Texture Properties");
			ImGui::BeginChild("Textures in current model");
			int size = mesh.GetTextures().size();
			for (int n = 0; n < size; n++)
			{
				ImGui::Text("%s", mesh.GetTextures().at(n)->GetFilePath().c_str());
				ImGui::Spacing();
			}
			ImGui::EndChild();
			if(ImGui::Button("Add New Texture"))
			{ }
			ImGui::End();
		}
	}

	void GraphXGui::LightProperties(Light& light)
	{
		if (light.bShowDetails)
		{
			ImGui::Begin("Lighting", (bool*)light.bShowDetails);
			ImGui::SliderFloat3("Position", (float*)&light.Position.x, -500.0f, 500.0f);
			ImGui::SliderFloat("Intensity", (float*)&light.Intensity, 0.0f, 100.0f);
			ImGui::ColorEdit4("Color", (float*)&light.Color);
			ImGui::End();
		}
	}

	void GraphXGui::CameraProperties(Camera& camera)
	{
		ImGui::Begin("Camera Properties", (bool*)true);
		ImGui::SliderFloat("Camera Speed", (float*)&camera.CameraSpeed, 0.0f, 100.0f);
		ImGui::SliderFloat("Field Of View", (float*)&camera.FieldOfView, 10.0f, 100.0f);
		ImGui::SliderFloat("Camera Sensitivity", (float*)&Mouse::GetMouse()->Sensitivity, 0.1f, 1.0f);
		ImGui::End();
	}

	void GraphXGui::DetailsWindow(Mesh2D& mesh)
	{
		if (mesh.bShowDetails)
		{
			ImGui::Begin("Details", (bool*)mesh.bShowDetails);

			ImGui::Text("Transformations");
			ImGui::SliderFloat3("Translation", (float*)&mesh.Position.x, -1000.0f, 1000.0f);
			ImGui::SliderFloat3("Rotation", (float*)(&mesh.Rotation), 0.0f, 359.f);
			ImGui::SliderFloat2("Scale", (float*)(&mesh.Scale.x), 0.0001f, 10.f);

			ImGui::Text("Color and Properties");
			ImGui::SliderFloat("Shininess", (float*)&mesh.Shininess, 2.0f, 256.0f);
			ImGui::SliderFloat("Reflectivity", (float*)&mesh.Reflectivity, 0.0f, 1.0f);
			ImGui::ColorEdit4("Base Color", (float*)&mesh.BaseColor);

			ImGui::End();
		}
	}

	void GraphXGui::LoadModel()
	{
		ImGui::Begin("Model Properties", (bool*)true);
		if(ImGui::Button("Cube"))
		{ }
		if (ImGui::Button("Add Model"))
		{ }
		ImGui::End();
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