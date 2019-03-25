#pragma once

#include "Entity.h"

namespace engine
{
	class Terrain
		: public Entity
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

		/* Blendmap used to draw the terrain */
		const class Texture* m_BlendMap;

		/* Textures used for the terrain */
		std::vector<const class Texture*> *m_Textures;

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
		Terrain(int Width, int Depth, float TileSize, const std::vector<std::string>& TexNames, const std::string& BlendMap, const gm::Vector3& Pos, const gm::Vector2& Scale);

		/** Entity Interface **/
		void Update(float DeltaTime) override;

		/* Prepares the terrain for rendering */
		void Enable() const;

		/* Unbinds the terrain after the rendering */
		void Disable() const override;

		/* Returns the mesh of the terrain */
		const class Mesh3D& GetMesh() const { return *m_Mesh; }

		/* Returns the shader used for the terrain */
		class Shader* GetShader() const { return m_Shader; }

		~Terrain();

	private:
		/* Builds the terrain */
		void BuildTerrain();
		
		void Enable(class Shader& shader, const std::string& Name = "u_Terrain") const override;

		double GetYCoords(int x, int y);
	    
		double SmoothNoise(int x, int y);

		double Interpolate(double a, double b, double blend);

		double InterpolatedNoise(double x, double y);

		gm::Vector3 CalculateNormal(double x, double y);
	};
}