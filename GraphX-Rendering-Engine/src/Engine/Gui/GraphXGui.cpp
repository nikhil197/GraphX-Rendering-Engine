#include "pch.h"
#include "GraphXGui.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"

#include "Model/Mesh/Mesh2D.h"
#include "Model/Mesh/Mesh3D.h"

#include "Textures/Texture2D.h"
#include "Materials/Material.h"

#include "Entities/Lights/PointLight.h"
#include "Entities/Camera.h"
#include "Entities/Terrain.h"
#include "Entities/Skybox.h"

#include "Engine/Controllers/CameraController.h"

#include "Utilities/FileOpenDialog.h"
#include "Utilities/EngineUtil.h"

#include "Input/Mouse.h"
#include "Events/GUIEvent.h"

namespace GraphX
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
		GX_PROFILE_FUNCTION()

		ImGui_ImplGlfwGL3_NewFrame();
	}

	void GraphXGui::GlobalSettings(const Ref<Skybox>& skybox, float& daytime, float& SunLightIntensity, bool& EnableParticles)
	{
		ImGui::Begin("Global Settings", (bool*)true);

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Skybox Details");
		ImGui::DragFloat("Rotation Speed", &skybox->RotationSpeed, 0.1f, 0.0f, 2.0f);

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Day Time Details");
		ImGui::DragFloat("Day Time Hours", &daytime, 0.1f, 0.0f, 2.0f);

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Global Light Intensity");
		ImGui::DragFloat("Global Light Intensity", &SunLightIntensity, 0.1f, 0.1f, 5.0f);

		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Enable Particle Effects");
		ImGui::Checkbox("Enable Particle Effects", &EnableParticles);
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

			static GM::Vector3 postion;
			static GM::Vector2 scale(1);
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
						CreateTerrainEvent e(CreateRef<Terrain>(x, z, tileSize, textures, "res/Textures/Terrain/BlendMap.png", postion, scale));
						textures.clear();
						s_GuiEventCallback(e);
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

	void GraphXGui::DetailsWindow(const Ref<Mesh3D>& mesh, const std::string& Name)
	{
		if (mesh->bShowDetails)
		{
			ImGui::Begin(Name.c_str(), (bool*)mesh->bShowDetails);
			ImGui::Spacing();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Transformations");
			if (ImGui::DragFloat3("Translation", &mesh->Position.x, 1.0f, -1000.0f, 1000.0f))
			{
				mesh->UpdateModelMatrix(true);
			}
			if (ImGui::DragFloat3("Rotation", (float*)& mesh->Rotation, 1.0f, -359.0f, 359.f))
			{
				mesh->UpdateModelMatrix(true);
			}
			if (ImGui::DragFloat3("Scaling", (float*)& mesh->Scale.x, 1.0f, 0.0001f, 10.f))
			{
				mesh->UpdateModelMatrix(true);
			}

			ImGui::Spacing();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Color and Properties");

			const Ref<Material>& Mat = mesh->GetMaterial();

			GM::Vector4 BaseColor = Mat->GetBaseColor();
			float Reflectivity = Mat->GetSpecularStrength();
			float Shininess = Mat->GetShininess();

			if (ImGui::DragFloat("Shininess", (float*)& Shininess, 1.0f, 2.0f, 256.0f))
			{
				Mat->SetShininess(Shininess);
			}

			if (ImGui::DragFloat("Reflectivity", (float*)& Reflectivity, 1.0f, 0.0f, 1.0f))
			{
				Mat->SetSpecularStrength(Reflectivity);
			}

			if (ImGui::ColorEdit4("Tint Color", (float*)& BaseColor))
			{
				Mat->SetBaseColor(BaseColor);
			}

			ImGui::Spacing();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Texture Properties");

			ImGui::BeginChild("Textures in current model");
			std::vector<Ref<const Texture2D>> Textures = Mat->GetTextures();
			int size = Textures.size();
			for (int n = 0; n < size; n++)
			{
				ImGui::Text("%s", Textures.at(n)->GetFilePath().c_str());
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

	void GraphXGui::LightProperties(const Ref<PointLight>& light)
	{
		if (light->bShowDetails)
		{
			ImGui::Begin("Lighting", (bool*)light->bShowDetails);
			ImGui::DragFloat3("Position", (float*)&light->Position.x, 1.0f, -500.0f, 500.0f);
			ImGui::DragFloat("Intensity", (float*)&light->Intensity, 1.0f, 0.0f, 100.0f);
			ImGui::ColorEdit4("Color", (float*)&light->Color);
			ImGui::End();
		}
	}

	void GraphXGui::CameraProperties(const Ref<CameraController>& cameraController)
	{
		float FOV = cameraController->GetFieldOfView();
		bool bPerspectiveMode = cameraController->GetProjectionMode() == ProjectionMode::Perspective;
		ImGui::Begin("Camera Properties", (bool*)true);
		ImGui::DragFloat("Camera Translation Speed", (float*)&cameraController->TranslationSpeed, 1.0f, 0.0f, 100.0f);
		ImGui::DragFloat("Camera Roll Speed", (float*)&cameraController->RollSpeed, 1.0f, 30.0f, 360.0f);

		if (ImGui::DragFloat("Field Of View", &FOV, .1f, 10.0f, 100.0f))
		{
			if (s_GuiEventCallback)
			{
				CameraFOVChangedEvent e(*cameraController->GetCamera(), FOV);
				s_GuiEventCallback(e);
			}
		}
		
		ImGui::DragFloat("Mouse Sensitivity", (float*)&Mouse::GetMouse()->Sensitivity, 0.01f, 0.01f, 1.0f);

		if (ImGui::RadioButton("Perspective", bPerspectiveMode))
		{
			if (s_GuiEventCallback)
			{
				CameraProjectionModeChange e(*cameraController->GetCamera(), ProjectionMode::Perspective);
				s_GuiEventCallback(e);
			}
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Orthographic", !bPerspectiveMode))
		{
			if (s_GuiEventCallback)
			{
				CameraProjectionModeChange e(*cameraController->GetCamera(), ProjectionMode::Orthographic);
				s_GuiEventCallback(e);
			}
		}
		ImGui::End();
	}

	void GraphXGui::DetailsWindow(const Ref<Mesh2D>& mesh)
	{
		if (mesh->bShowDetails)
		{
			ImGui::Begin("Details", (bool*)mesh->bShowDetails);

			ImGui::Text("Transformations");
			ImGui::SliderFloat3("Translation", (float*)&mesh->Position.x, -1000.0f, 1000.0f);
			ImGui::SliderFloat3("Rotation", (float*)(&mesh->Rotation), 0.0f, 359.f);
			ImGui::SliderFloat2("Scale", (float*)(&mesh->Scale.x), 0.0001f, 10.f);

			ImGui::Text("Color and Properties");
			
			Ref<Material> Mat = mesh->GetMaterial();
			GM::Vector4 BaseColor = Mat->GetBaseColor();
			float Reflectivity = Mat->GetSpecularStrength();
			float Shininess = Mat->GetShininess();

			if (ImGui::DragFloat("Shininess", (float*)&Shininess, 1.0f, 2.0f, 256.0f))
			{
				Mat->SetShininess(Shininess);
			}

			if (ImGui::DragFloat("Reflectivity", (float*)&Reflectivity, 1.0f, 0.0f, 1.0f))
			{
				Mat->SetSpecularStrength(Reflectivity);
			}

			if (ImGui::ColorEdit4("Tint Color", (float*)&BaseColor))
			{
				Mat->SetBaseColor(BaseColor);
			}

			ImGui::End();
		}
	}

	void GraphXGui::TerrainDetails(const Ref<Terrain>& terrain)
	{
		if (true)
		{
			ImGui::Begin("Terrain Details", (bool*)true);
			if (ImGui::DragFloat3("Position", (float*)&terrain->GetMesh()->Position.x, 1.0f, -1000.0f, 1000.0f))
			{
				Mesh3D& mesh = const_cast<Mesh3D&>(*terrain->GetMesh());
				mesh.UpdateModelMatrix(true);
			}
			ImGui::DragFloat3("Scale in X & Z", (float*)&terrain->GetMesh()->Scale.x, 1.0f, 0.0001f, 10.f);
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

	bool GraphXGui::WantsMouseInput()
	{
		return ImGui::GetIO().WantCaptureMouse;
	}

	void GraphXGui::Cleanup()
	{
		GX_ENGINE_INFO("GraphXGui: Cleaning up ImGui");
		ImGui_ImplGlfwGL3_Shutdown();
		ImGui::DestroyContext();
	}
}