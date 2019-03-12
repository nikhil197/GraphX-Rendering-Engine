#include "pch.h"
#include "Terrain.h"

#include "Model/Mesh/Vertex.h"
#include "Model/Mesh/Mesh3D.h"

#include "Shaders/Shader.h"
#include "Textures/Texture.h"

#include "VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"

namespace engine
{
	Terrain::Terrain(int Width, int Depth, float TileSize, const std::vector<std::string>& TexNames)
		: m_Mesh(nullptr), m_Shader(nullptr), m_Width(Width), m_Depth(Depth), m_TileSize(TileSize), m_Vertices(nullptr), m_Indices(nullptr), m_Textures(nullptr)
	{
		BuildTerrain();
	
		m_Shader = new Shader("res/Shaders/Terrain.shader");

		if (TexNames.size() > 0)
			m_Textures = new std::vector<const Texture*>();

		for (unsigned int i = 0; i < TexNames.size(); i++)
		{
			const Texture* tex = new Texture(TexNames[i], true); // All terrain textures will be tiled textures
			m_Textures->emplace_back(tex);
		}
		
		if (m_Textures && m_Vertices && m_Indices)
			m_Mesh = new Mesh3D(gm::Vector3::ZeroVector, gm::Vector3::ZeroVector, gm::Vector3::UnitVector, *m_Shader, *m_Textures, *m_Vertices, *m_Indices, gm::Vector4::ZeroVector, -1.0f, -1.0f);
		else
			GX_ENGINE_ERROR("Error while building the terrain");
	}

	void Terrain::BuildTerrain()
	{
		m_Vertices = new std::vector<Vertex3D>();
		m_Indices = new std::vector<unsigned int>();

		Vertex3D vertex;
		for (int z = 0; z < m_Depth; z++)
		{
			for (int x = 0; x < m_Width; x++)
			{
				// Calculate the vertices of the terrain
				vertex.Position = gm::Vector3(x * m_TileSize, -10.0f, -z * m_TileSize);	// TODO: Add the height for the terrain
				vertex.Normal   = gm::Vector3(0.0f, 1.0f, 0.0f);	// TODO: Change the normals when flat terrain is replaced with height maps
				vertex.TexCoord = gm::Vector2((float)x, (float)z);
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
	}

	void Terrain::Update(float DeltaTime)
	{

	}

	void Terrain::Enable(class Shader& shader, const std::string& Name) const
	{
	}

	void Terrain::Enable() const
	{
		if (m_Mesh)
		{
			m_Mesh->Enable();
		}
	}

	void Terrain::Disable() const
	{
		if (m_Mesh)
		{
			m_Mesh->Disable();
		}
	}

	Terrain::~Terrain()
	{
		delete m_Mesh;

		for (unsigned int i = 0; i < m_Textures->size(); i++)
		{
			delete m_Textures->at(i);
		}

		delete m_Vertices;
		delete m_Indices;
	}
}