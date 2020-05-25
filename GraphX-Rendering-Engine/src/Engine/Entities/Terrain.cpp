#include "pch.h"
#include "Terrain.h"

#include "Utilities/EngineUtil.h"
#include "Model/Mesh/Mesh3D.h"

#include "Engine/Core/Vertex.h"
#include "Shaders/Shader.h"
#include "Materials/Material.h"
#include "Textures/Texture2D.h"

#include "VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"


namespace GraphX
{
	using namespace GM;

	double SmoothNoise(int x, int z)
	{
		static unsigned long long seed = 7436767332u;
		double corners = EngineUtil::Rand(x - 1, z - 1, seed) + EngineUtil::Rand(x + 1, z - 1, seed)
			+ EngineUtil::Rand(x - 1, z + 1, seed) + EngineUtil::Rand(x + 1, z + 1, seed) / 16.0;

		double sides = EngineUtil::Rand(x - 1, z, seed) + EngineUtil::Rand(x + 1, z, seed)
			+ EngineUtil::Rand(x, z + 1, seed) + EngineUtil::Rand(x, z - 1, seed) / 8.0;

		double center = EngineUtil::Rand(x, z, seed) / 4.0;
		return corners + sides + center;
	}

	double InterpolatedNoise(double x, double z)
	{
		GX_PROFILE_FUNCTION()

		int intX = (int)x;
		int intZ = (int)z;
		double fracX = x - intX;
		double fracZ = z - intZ;

		double v1 = SmoothNoise(intX, intZ);
		double v2 = SmoothNoise(intX + 1, intZ);
		double v3 = SmoothNoise(intX, intZ + 1);
		double v4 = SmoothNoise(intX + 1, intZ + 1);

		float blend = (float)(1.0 - std::cos(fracX * PI)) * 0.5f;
		double i1 = Utility::Lerp(v1, v2, blend);
		double i2 = Utility::Lerp(v3, v4, blend);

		blend = (float)(1.0 - std::cos(fracZ * PI)) * 0.5f;
		return Utility::Lerp(i1, i2, blend);
	}

	double Terrain::s_Amplitude = 5.0;

	Terrain::Terrain(int TilesX, int TilesZ, float TileSize, const std::vector<std::string>& TexNames, const std::string& BlendMap, const Vector3& Position, const Vector2& Scale)
		: m_Mesh(nullptr), m_Material(nullptr), m_TilesX(TilesX), m_TilesZ(TilesZ), m_TileSize(TileSize), m_Vertices(nullptr), m_Indices(nullptr)
	{
		GX_PROFILE_FUNCTION()

		BuildTerrain();
		Ref<Shader> shader = CreateRef<Shader>("res/Shaders/TerrainShader.glsl");
		shader->Bind();
		shader->SetUniform2i("u_TerrainDimensions", m_TilesX, m_TilesZ);
		shader->SetUniform1f("u_AmbientStrength", 0.01f);

		m_Material = CreateRef<Material>(shader);
		m_Material->SetSpecularStrength(1.0f);
		m_Material->SetShininess(256.0f);
		
		m_BlendMap = CreateRef<const Texture2D>(BlendMap);
		
		if (TexNames.size() > 0)
		{
			for (unsigned int i = 0; i < TexNames.size(); i++)
			{
				Ref<const Texture2D> tex = CreateRef<const Texture2D>(TexNames[i], true); // All terrain textures will be tiled textures
				m_Material->AddTexture(tex);
			}
		}
		
		if (m_Vertices && m_Indices)
		{
			m_Mesh = CreateRef<Mesh3D>(Position, Rotator::ZeroRotator, Vector3(Scale.x, 1.0f, Scale.y), *m_Vertices, *m_Indices, m_Material);
			
			delete m_Vertices;
			delete m_Indices;
		}
		else
		{
			GX_ENGINE_ERROR("Error while building the terrain");
		}
	}

	void Terrain::BuildTerrain()
	{
		Timer timer("Build Terrain");
		GX_ENGINE_INFO("Building Terrain");
		GX_PROFILE_FUNCTION()
		
		m_Vertices = new std::vector<Vertex3D>();
		m_Indices = new std::vector<unsigned int>();
		Vertex3D vertex;
		for (int z = 0; z < m_TilesZ; z++)
		{
			for (int x = 0; x < m_TilesX; x++)
			{
				// Calculate the vertices of the terrain
				//double yCoord = GetYCoords(x, z);
				vertex.Position = Vector3(x * m_TileSize, /*(float)yCoord*/-10.0f, -z * m_TileSize);
				vertex.TexCoord = Vector2((float)x, (float)z);
				m_Vertices->emplace_back(vertex);

				// Calculate the indices for the vertices of the terrain
				if (x != m_TilesX - 1 && z != m_TilesZ - 1)
				{
					// Lower triangle
					m_Indices->push_back(z * m_TilesX + x);
					m_Indices->push_back(z * m_TilesX + x + 1);
					m_Indices->push_back(((z + 1) * m_TilesX) + x + 1);

					// Upper triangle
					m_Indices->push_back(((z + 1) * m_TilesX) + x + 1);
					m_Indices->push_back(((z + 1) * m_TilesX) + x);
					m_Indices->push_back(z * m_TilesX + x);
				}
			}
		}

		for (int z = 0; z < m_TilesZ; z++)
		{
			for (int x = 0; x < m_TilesX; x++)
			{
				CalculateNormal(x, z);
			}
		}

		// Reset the seed back to default
		EngineUtil::ResetSeed();
	}
	

	double Terrain::GetYCoords(int x, int z)
	{
		double total = InterpolatedNoise(x / 8.0 , z / 8.0 ) * s_Amplitude;
		total += InterpolatedNoise(x / 4.0 , z / 4.0 ) * s_Amplitude / 3.0 ;
		total += InterpolatedNoise(x / 2.0 , z / 2.0 ) * s_Amplitude / 9.0 ;
		return total;
	}

	void Terrain::CalculateNormal(int x, int z)
	{
		GX_PROFILE_FUNCTION()

		float heightL = m_Vertices->at(Utility::Max((x - 1) + z * m_TilesX, 0)).Position.y;
		float heightR = m_Vertices->at(Utility::Min((x + 1) + z * m_TilesX, (int)m_Vertices->size() - 1)).Position.y;
		float heightD = m_Vertices->at(Utility::Max(x + (z - 1) * m_TilesX, 0)).Position.y;
		float heightU = m_Vertices->at(Utility::Min(x + (z + 1) * m_TilesX, (int)m_Vertices->size() - 1)).Position.y;
		m_Vertices->at(x + z * m_TilesX).Normal = Vector3(heightL - heightR, 2.0, heightD - heightU).Normal();
	}
	
	void Terrain::Update(float DeltaTime)
	{
		if (m_Mesh)
		{
			m_Mesh->Update(DeltaTime);
		}
	}

	void Terrain::Enable(class Shader& shader, const std::string& Name) const
	{
	}

	void Terrain::Enable() const
	{
		GX_PROFILE_FUNCTION()

		m_Material->Bind();
		m_BlendMap->Bind(4);
		m_Material->GetShader()->SetUniform1i("u_BlendMap", 4);

		if (m_Mesh)
		{
			m_Mesh->Enable();
		}
	}

	void Terrain::Disable() const
	{
		GX_PROFILE_FUNCTION()

		m_BlendMap->UnBind();
		if (m_Mesh)
		{
			m_Mesh->Disable();
		}
	}

	Terrain::~Terrain()
	{
	}
}