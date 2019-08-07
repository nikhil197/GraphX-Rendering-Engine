#include "pch.h"
#include "Terrain.h"

#include "Utilities/EngineUtil.h"
#include "Model/Mesh/Vertex.h"
#include "Model/Mesh/Mesh3D.h"
#include "Shaders/Shader.h"
#include "Textures/Texture.h"

#include "VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"


namespace GraphX
{   
	double Terrain::s_Amplitude = 5.0;

	Terrain::Terrain(int Width, int Depth, float TileSize, const std::vector<std::string>& TexNames, const std::string& BlendMap, const GraphXMaths::Vector3& Position, const GraphXMaths::Vector2& Scale)
		: m_Mesh(nullptr), m_Shader(nullptr), m_Width(Width), m_Depth(Depth), m_TileSize(TileSize), m_Vertices(nullptr), m_Indices(nullptr), m_Textures(nullptr)
	{
		BuildTerrain();
	
		m_Shader = new Shader("res/Shaders/Terrain.shader");
		m_Shader->Bind();
		m_Shader->SetUniform2i("u_TerrainDimensions", m_Width, m_Depth);
		m_Shader->UnBind();

		if (TexNames.size() > 0)
			m_Textures = new std::vector<const Texture*>();

		for (unsigned int i = 0; i < TexNames.size(); i++)
		{
			const Texture* tex = new Texture(TexNames[i], true); // All terrain textures will be tiled textures
			m_Textures->emplace_back(tex);
		}

		m_BlendMap = new Texture(BlendMap);
		
		if (m_Textures && m_Vertices && m_Indices)
		{
			m_Mesh = new Mesh3D(Position, GraphXMaths::Vector3::ZeroVector, GraphXMaths::Vector3(Scale.x, 1.0f, Scale.y), m_Shader, *m_Textures, *m_Vertices, *m_Indices, GraphXMaths::Vector4::ZeroVector, -1.0f, -1.0f);
			
			delete m_Vertices;
			delete m_Indices;
		}
		else
			GX_ENGINE_ERROR("Error while building the terrain");
	}

	void Terrain::BuildTerrain()
	{
		Timer timer("Building Terrain");

		m_Vertices = new std::vector<Vertex3D>();
		m_Indices = new std::vector<unsigned int>();
		Vertex3D vertex;
		for (int z = 0; z < m_Depth; z++)
		{
			for (int x = 0; x < m_Width; x++)
			{
				// Calculate the vertices of the terrain
				double yCoord = GetYCoords(x, z);
				vertex.Position = GraphXMaths::Vector3(x * m_TileSize, (float)yCoord, -z * m_TileSize);
				vertex.TexCoord = GraphXMaths::Vector2((float)x, (float)z);
				m_Vertices->emplace_back(vertex);

				// Calculate the indices for the vertices of the terrain
				if (x != m_Width - 1 && z != m_Depth - 1)
				{
					// Lower triangle
					m_Indices->push_back(z * m_Width + x);
					m_Indices->push_back(z * m_Width + x + 1);
					m_Indices->push_back(((z + 1) * m_Width) + x + 1);

					// Upper triangle
					m_Indices->push_back(((z + 1) * m_Width) + x + 1);
					m_Indices->push_back(((z + 1) * m_Width) + x);
					m_Indices->push_back(z * m_Width + x);
				}
			}
		}

		for (int z = 0; z < m_Depth; z++)
		{
			for (int x = 0; x < m_Width; x++)
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

	double Terrain::SmoothNoise(int x, int z)
	{
		static unsigned long long seed = 7436767332u;
		double corners = EngineUtil::GetRandomValue(x - 1, z - 1, seed) + EngineUtil::GetRandomValue(x + 1, z - 1, seed) + EngineUtil::GetRandomValue(x - 1, z + 1, seed) + EngineUtil::GetRandomValue(x + 1, z + 1, seed) / 16.0;
	    double sides = EngineUtil::GetRandomValue(x - 1, z , seed) + EngineUtil::GetRandomValue(x + 1, z , seed) + EngineUtil::GetRandomValue(x , z + 1, seed) + EngineUtil::GetRandomValue(x , z - 1, seed) / 8.0;
		double center = EngineUtil::GetRandomValue(x, z, seed) / 4.0;
		return corners + sides + center;
	}

	double Terrain::Interpolate(double a, double b, double blend)
	{
		double theta = blend * GraphXMaths::PI;
		double f = (1.0 - std::cos(theta)) * 0.5;
		return a * (1.0 - f) + b * f;
	}

	double Terrain::InterpolatedNoise(double x, double z)
	{
		int intX = (int)x;
		int intZ = (int)z;
		double fracX = x - intX;
		double fracZ = z - intZ; 

		double v1 = SmoothNoise(intX, intZ);
		double v2 = SmoothNoise(intX + 1, intZ);
		double v3 = SmoothNoise(intX, intZ + 1);
		double v4 = SmoothNoise(intX + 1 , intZ + 1);

		double i1 = Interpolate(v1, v2, fracX);
		double i2 = Interpolate(v3, v4, fracX);
		return Interpolate(i1, i2, fracZ);
	}

	void Terrain::CalculateNormal(int x, int z)
	{
		float heightL = m_Vertices->at(GraphXMaths::Utility::Max((x - 1) + z * m_Width, 0)).Position.y;
		float heightR = m_Vertices->at(GraphXMaths::Utility::Min((x + 1) + z * m_Width, (int)m_Vertices->size() - 1)).Position.y;
		float heightD = m_Vertices->at(GraphXMaths::Utility::Max(x + (z - 1) * m_Width, 0)).Position.y;
		float heightU = m_Vertices->at(GraphXMaths::Utility::Min(x + (z + 1) * m_Width, (int)m_Vertices->size() - 1)).Position.y;
		m_Vertices->at(x + z * m_Width).Normal = GraphXMaths::Vector3(heightL - heightR, 2.0, heightD - heightU).Normal();
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
		m_Shader->Bind();
		m_BlendMap->Bind(4);
		m_Shader->SetUniform1i("u_BlendMap", 4);

		if (m_Mesh)
		{
			m_Mesh->Enable();
		}
	}

	void Terrain::Disable() const
	{
		m_BlendMap->UnBind();
		if (m_Mesh)
		{
			m_Mesh->Disable();
		}

		m_Shader->UnBind();
	}

	Terrain::~Terrain()
	{
		delete m_Mesh;
	}
}