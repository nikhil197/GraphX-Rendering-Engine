#pragma once

#include "Entity.h"

namespace GraphX
{
	class Mesh3D;
	class Material;
	class Texture2D;

	class Terrain
		: public Entity
	{
	public:
		/* Constructs a new terrain */
		/* @Param TilesX  - # of tiles in the x - direction */
		/* @Param TilesZ - # of tiles in the negative z - direction */
		/* @Param TexNames - Names of the textures to be used for the terrain */
		/* @Param BlendMap - Name of the blend map used to sample the textures specified in TexNames */
		/* @Param Pos - Position of the terrain in the world */
		/* @Param Scale - Scale of the mesh in the x and z direction */
		Terrain(int TilesX, int TilesZ, float TileSize, const std::vector<std::string>& TexNames, const std::string& BlendMap, const GM::Vector3& Pos, const GM::Vector2& Scale);

		/** Entity Interface **/
		void Update(float DeltaTime) override;

		/* Prepares the terrain for rendering */
		void Enable() const;

		/* Unbinds the terrain after the rendering */
		virtual void Disable() const override;

		/* Returns the mesh of the terrain */
		inline Ref<Mesh3D> GetMesh() const { return m_Mesh; }

		/* Returns the shader used for the terrain */
		inline Ref<Material> GetMaterial() const { return m_Material; }

		/* Returns the Width of the terrain (x - direction) */
		inline float GetWidth() const { return m_TileSize * m_TilesX; }

		/* Returns the depth of the terrain (z - direction) */
		inline float GetDepth() const { return m_TileSize * m_TilesZ; }

		virtual ~Terrain();

	private:
		/* Builds the terrain */
		void BuildTerrain();
		
		virtual void Enable(class Shader& shader, const std::string& Name = "") const override;

		/* Calculates the y - Coordinate for the vertices of the terrain mesh */
		double GetYCoords(int x, int y);

		/* Calculates normals for the created mesh of the terrain */
		void CalculateNormal(int x, int y);

	private:
		/* Mesh to represent the terrain */
		Ref<Mesh3D> m_Mesh;

		/* Material used to render terrain */
		Ref<Material> m_Material;

		/* # tiles in X and Z direction respectively */
		const int m_TilesX, m_TilesZ;

		/* Tile size for the terrain */
		float m_TileSize;

		/* Vertices of the terrain mesh */
		std::vector<struct Vertex3D>* m_Vertices;

		/* Indices for the terrain mesh */
		std::vector<unsigned int>* m_Indices;

		/* Blendmap used to draw the terrain */
		Ref<const Texture2D> m_BlendMap;

		/* Amplitude of the height map for the terrain */
		static double s_Amplitude;
	};
}