#include "pch.h"
#include "GraphXGui.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"

#include "model/mesh/Mesh3D.h"
#include "model/mesh/Mesh2D.h"

#include "Textures/Texture.h"

#include "entities/Lights/Light.h"
#include "entities/Camera.h"
#include "entities/Terrain.h"
#include "entities/Skybox.h"

#include "Utilities/FileOpenDialog.h"
#include "Utilities/EngineUtil.h"

#include "Input/Mouse.h"
#include "Events/GUIEvent.h"

namespace engine
{
	std::function<void(Event&)> GraphXGui::s_GuiEventCallback = nullptr;

	void GraphXGui::Init(GLFWwindow* window, const std::function<void(Event&)>& callback, bool bSetupCallbacks)
	{
		Timer timer("Initialising ImGui");

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, bSetupCallbacks);

		ImGui::StyleColorsDark();

		s_GuiEventCallback = callback;
	}

	void GraphXGui::Update()
	{
		ImGui_ImplGlfwGL3_NewFrame();
	}

	void GraphXGui::GlobalSettings(Skybox& skybox, float& daytime)
	{
		ImGui::Begin("Global Settings", (bool*)true);

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Skybox Details");
		ImGui::DragFloat("Rotation Speed", &skybox.RotationSpeed, 0.1f, 0.0f, 2.0f);

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Day Time Details");
		ImGui::DragFloat("Day Time Hours", &daytime, 0.1f, 0.0f, 2.0f);
		ImGui::End();
	}

	void GraphXGui::AddTerrain()
	{
		if (ImGui::Button("Add Terrain"))
		{
			ImGui::OpenPopup("Terrain");
		}

		if (ImGui::BeginPopupModal("Terrain", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Separator();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::PopStyleVar();

			static gm::Vector3 postion;
			static gm::Vector2 scale(1);
			static int x = 0, z = 0;
			static float tileSize = 0.0f;
			static std::vector<std::string> textures;
			ImGui::Text("Terrain Attributes");
			ImGui::InputInt("No. of Tiles in X", &x);
			ImGui::InputInt("No. of Tiles in Z", &z);
			ImGui::DragFloat("Tile Size", &tileSize, 0.5f, 1.0f, 10.f);
			if (ImGui::Button("Add Texture"))
			{
				FileOpenDialog dialogBox(ResourceType::TEXTURES);
				dialogBox.Show();

				const std::string& Tex = EngineUtil::ToByteString(dialogBox.GetAbsolutePath());
				textures.push_back(Tex);
			}
			ImGui::DragFloat3("Position", &postion.x, 1.0f, -1000.0f, 1000.0f);
			ImGui::DragFloat2("Scale X and Z", &scale.x, 0.5f, 0.5f, 10.0f);

			if (ImGui::Button("Create Terrain", ImVec2(120, 0))) 
			{
				if (x > 0 && z > 0 && tileSize > 0.0f)
				{
					if (s_GuiEventCallback)
					{
						//CreateTerrainEvent e(new Terrain(x, z, tileSize, textures, postion, scale));
						//textures.clear();
						//s_GuiEventCallback(e);
					}
				}
				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{ 
				textures.clear();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
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
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Transformations");
			ImGui::DragFloat3("Translation", &mesh.Position.x, 1.0f, -1000.0f, 1000.0f);
			ImGui::DragFloat3("Rotation", (float*)&mesh.Rotation, 1.0f, 0.0f, 359.f);
			ImGui::DragFloat3("Scaling", (float*)&mesh.Scale.x, 1.0f, 0.0001f, 10.f);

			ImGui::Spacing();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Color and Properties");
			ImGui::DragFloat("Shininess", (float*)&mesh.Shininess, 1.0f, 2.0f, 256.0f);
			ImGui::DragFloat("Reflectivity", (float*)&mesh.Reflectivity, 1.0f, 0.0f, 1.0f);

			ImGui::ColorEdit4("Base Color", (float*)&mesh.BaseColor);
			ImGui::Spacing();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture Properties");

			ImGui::BeginChild("Textures in current model");
			int size = mesh.GetTextures().size();
			for (int n = 0; n < size; n++)
			{
				ImGui::Text("%s", mesh.GetTextures().at(n)->GetFilePath().c_str());
			}
			if(ImGui::Button("Add New Texture"))
			{
				// Add a new texture to the mesh
				if (s_GuiEventCallback)
				{
					static AddTextureEvent e;
					s_GuiEventCallback(e);
				}
			}
			ImGui::EndChild();
			ImGui::End();
		}
	}

	void GraphXGui::LightProperties(Light& light)
	{
		if (light.bShowDetails)
		{
			ImGui::Begin("Lighting", (bool*)light.bShowDetails);
			ImGui::DragFloat3("Position", (float*)&light.Position.x, 1.0f, -500.0f, 500.0f);
			ImGui::DragFloat("Intensity", (float*)&light.Intensity, 1.0f, 0.0f, 100.0f);
			ImGui::ColorEdit4("Color", (float*)&light.Color);
			ImGui::End();
		}
	}

	void GraphXGui::CameraProperties(Camera& camera)
	{
		ImGui::Begin("Camera Properties", (bool*)true);
		ImGui::DragFloat("Camera Speed", (float*)&camera.CameraSpeed, 1.0f, 0.0f, 100.0f);
		if (ImGui::DragFloat("Field Of View", (float*)&camera.FieldOfView, 1.0f, 10.0f, 100.0f))
		{
			if (s_GuiEventCallback)
			{
				CameraFOVChangedEvent e(camera);
				s_GuiEventCallback(e);
			}
		}
		ImGui::DragFloat("Camera Sensitivity", (float*)&Mouse::GetMouse()->Sensitivity, 0.01f, 0.01f, 5.0f);
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

	void GraphXGui::TerrainDetails(Terrain& terrain)
	{
		if (true)
		{
			ImGui::Begin("Terrain Details", (bool*)true);
			ImGui::DragFloat3("Position", (float*)&terrain.GetMesh().Position.x, 1.0f, -1000.0f, 1000.0f);
			ImGui::DragFloat3("Scale in X & Z", (float*)&terrain.GetMesh().Scale.x, 1.0f, 0.0001f, 10.f);
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Note: Scaling in y - direction is not advised ");
			ImGui::End();
		}
	}

	void GraphXGui::Models()
	{
		ImGui::Begin("Models", (bool*)true);
		if(ImGui::Button("Cube"))
		{
			// Add Cube to the scene
			if (s_GuiEventCallback)
			{
				AddModelEvent e(ModelType::CUBE);
				s_GuiEventCallback(e);
			}
		}
		if (ImGui::Button("Add Model"))
		{
			// Load a new, custom Model from the file system
			if (s_GuiEventCallback)
			{
				AddModelEvent e(ModelType::CUSTOM);
				s_GuiEventCallback(e);
			}
		}
		AddTerrain();

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