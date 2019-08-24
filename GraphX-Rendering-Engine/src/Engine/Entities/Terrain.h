#pragma once

#include "Entity.h"

namespace GraphX
{
	class Terrain
		: public Entity
	{
	private:
		/* Mesh to represent the terrain */
		class Mesh3D* m_Mesh;

		/* Material used to render terrain */
		class Material* m_Material;

		/* Width and depth of the terrain (no of tiles in X and Z direction respectively) */
		const int m_Width, m_Depth;

		/* Tile size for the terrain */
		float m_TileSize;

		/* Vertices of the terrain mesh */
		std::vector<struct Vertex3D>* m_Vertices;

		/* Indices for the terrain mesh */
		std::vector<unsigned int>* m_Indices;

		/* Blendmap used to draw the terrain */
		const class Texture* m_BlendMap;

		/* Amplitude of the height map for the terrain */
		static double s_Amplitude;

	public:
		/* Constructs a new terrain */
		/* @Param Width  - # of tiles in the x - direction */
		/* @Param Depth - # of tiles in the negative z - direction */
		/* @Param TexNames - Names of the textures to be used for the terrain */
		/* @Param BlendMap - Name of the blend map used to sample the textures specified in TexNames */
		/* @Param Pos - Position of the terrain in the world */
		/* @Param Scale - Scale of the mesh in the x and z direction */
		Terrain(int Width, int Depth, float TileSize, const std::vector<std::string>& TexNames, const std::string& BlendMap, const GraphXMaths::Vector3& Pos, const GraphXMaths::Vector2& Scale);

		/** Entity Interface **/
		void Update(float DeltaTime) override;

		/* Prepares the terrain for rendering */
		void Enable() const;

		/* Unbinds the terrain after the rendering */
		virtual void Disable() const override;

		/* Returns the mesh of the terrain */
		const class Mesh3D& GetMesh() const { return *m_Mesh; }

		/* Returns the shader used for the terrain */
		class Material* GetMaterial() const { return m_Material; }

		/* Returns the Width of the terrain (x - direction) */
		inline float GetWidth() const { return m_TileSize * m_Width; }

		/* Returns the depth of the terrain (z - direction) */
		inline float GetDepth() const { return m_TileSize * m_Depth; }

		virtual ~Terrain();

	private:
		/* Builds the terrain */
		void BuildTerrain();
		
		virtual void Enable(class Shader& shader, const std::string& Name = "") const override;

		/* Calculates the y - Coordinate for the vertices of the terrain mesh */
		double GetYCoords(int x, int y);
	    
		double SmoothNoise(int x, int y);

		double Interpolate(double a, double b, double blend);

		double InterpolatedNoise(double x, double y);

		/* Calculates normals for the created mesh of the terrain */
		void CalculateNormal(int x, int y);
	};
}