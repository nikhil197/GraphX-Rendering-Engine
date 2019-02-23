#pragma once

namespace engine
{
	class Terrain
	{
	private:
		/* 2D Mesh to represent the terrain */
		class Mesh3D* m_Mesh;

		/* Shader for the terrain */
		class Shader* m_Shader;

		/* Width and depth of the terrain (no of tiles in X and Z direction respectively) */
		int m_Width, m_Depth;

		/* Tile size for the terrain */
		float m_TileSize;

		/* Vertices of the terrain mesh */
		std::vector<struct Vertex3D>* m_Vertices;

		/* Indices for the terrain mesh */
		std::vector<unsigned int>* m_Indices;

		/* Textures used for the terrain */
		std::vector<const class Texture*> *m_Textures;

	public:
		/* Constructs a new terrain */
		/* @Param width  - # of tiles in the x - direction */
		/* @Param height - # of tiles in the negative z - direction */
		Terrain(int Width, int Height, float TileSize, const std::vector<std::string>& TexNames);

		/* Prepares the terrain for rendering */
		void Enable() const;

		/* Unbinds the terrain after the rendering */
		void Disable() const;

		/* Returns the mesh of the terrain */
		const class Mesh3D& GetMesh() const { return *m_Mesh; }

		/* Returns the shader used for the terrain */
		class Shader* GetShader() const { return m_Shader; }

		~Terrain();

	private:
		/* Builds the terrain */
		void BuildTerrain();
	};
}