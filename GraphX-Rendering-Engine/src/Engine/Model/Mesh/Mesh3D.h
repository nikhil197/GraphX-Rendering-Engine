#pragma once

namespace GraphX
{
	class Material;
	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;

	/** Raw data that makes up the mesh */
	struct RawMeshData
	{
		/* Vertex data of the mesh */
		std::vector<struct Vertex3D> Vertices;

		/* Indices into the Vertices array (for triangle render primitive), for rendering the mesh using triangles */
		std::vector<uint32_t> Indices;

		struct MeshSectionInfo
		{
			/* Index into the Vertices container at which a section starts */
			uint32_t SectionStartIndex = 0;

			/* Index into the materials array of the mesh3d. Material used by the section */
			uint32_t MaterialIndex = 0;
		};

		/* Indices at which a new section begins (If SectionStartIndices.size() == 0, then there is only one section in the mesh) */
		std::vector<MeshSectionInfo> SectionInfos;

		RawMeshData() = default;

		/* Constructor (In case there is only one section, passing in a section start indices vector is not necessary) */
		RawMeshData(const std::vector<Vertex3D>& verts, const std::vector<uint32_t>& indices);

		/* */
		RawMeshData(const std::vector<Vertex3D>& verts, const std::vector<uint32_t>& indices, const std::vector<MeshSectionInfo>& sectionStartIndices);
	};

	/* Information describing a section in Mesh3D */
	struct MeshSection
	{
		/** The index of the material with which to render this section. */
		uint32_t MaterialIndex;

		/** Range of vertices and indices used when rendering this section. */
		uint32_t FirstIndex;
		uint32_t NumTriangles;

		MeshSection()
			: MaterialIndex(0), FirstIndex(0), NumTriangles(0)
		{}
	};

	/* Mapping of material to all the sections of the mesh using that material (Can be one to one or one to many) */
	struct MaterialToSectionMap
	{
	public:
		/* Adds a section to the given material index */
		void Add(const uint32_t MaterialIndex, const uint32_t SectionIndex);

		/* Returns if there is an entry in the map with the given index as key */
		bool Exists(const uint32_t MaterialIndex) const;

		/* Returns all the section indices which use the material at the given material index */
		const std::vector<uint32_t>& GetSections(const uint32_t MaterialIndex) const;

		/* Returns section index for a particular section (numbering starting from 1), which uses the material at the given material index */
		uint32_t GetSection(const uint32_t MaterialIndex, const uint32_t SectionNum) const;

	private:
		/* Map from material index to section indices */
		/* Key - Material Index */
		/* Value - All the sections using this mateiral */
		std::unordered_map<uint32_t, std::vector<uint32_t>> m_Map;
	};

	/* Data required by the renderer to render the static mesh */
	struct RenderDataMesh3D
	{
		/* Vertex Array Object for the Mesh */
		Scope<VertexArray> VAO;

		/* Vertex Buffer for the object */
		Ref<VertexBuffer> VBO;

		/* Index Buffer for the Mesh */
		Ref<IndexBuffer> IBO;

		/* All the sections of the mesh */
		std::vector<MeshSection> Sections;
	};

	/* 3D Representation of an object in the scene */
	class Mesh3D
	{
	public:
		/* Loads meshes from a file 
		*  @param FilePath Asset file path
		*  @param Mat Material to be used for the meshes
		*/
		static Ref<Mesh3D> Load(const std::string& FilePath, const Ref<Material>& Mat);

	public:
		/**
		@param Pos Position of the mesh in the world
		@param Rotation Rotation of the mesh in the world
		@param Scale Scale of the mesh
		@param Vertices vertices of the mesh (counter clockwise order)
		@param Indices indices into the vertices vector
		@param Mat Material used to render the mesh
		*/
		Mesh3D(const GM::Vector3& Pos, const GM::Rotator& Rotation, const GM::Vector3& Scale, const std::vector<struct Vertex3D>& Vertices, const std::vector<unsigned int>& Indices, const Ref<Material>& Mat = nullptr);

		/*
		* @param Pos Position of the mesh in the world
		* @param Rotation Rotation of the mesh in the world
		* @param Scale Scale of the mesh
		* @param RawData Raw data from which the mesh will be built
		* @param Mat Material used to render the mesh
		*/
		Mesh3D(const GM::Vector3& Pos, const GM::Rotator& Rotation, const GM::Vector3& Scale, const RawMeshData* RawData = nullptr, const Ref<Material>& Mat = nullptr);

		// Copy Constructor
		Mesh3D(const Mesh3D& Mesh);

		/* Updates the status of the Mesh */
		virtual void Update(float DeltaTime);

		/* Prepares the object to be rendered */
		virtual void Enable() const;

		/* Unbinds all the buffers for the object */
		virtual void Disable() const;

		/* Returns the ibo for the object */
		inline Ref<const IndexBuffer> GetIBO() const { return m_RenderData->IBO; }

		/* Returns the number of sections in the mesh (including the base mesh) */
		inline uint32_t GetNumSections() const { return (uint32_t)m_RenderData->Sections.size(); }

		/* Returns the number of materials in the mesh */
		inline uint32_t GetNumMaterials() const { return (uint32_t)m_Materials.size(); }

		inline const MeshSection& GetMeshSection(uint32_t SectionIndex) const
		{
			GX_ENGINE_ASSERT(SectionIndex < m_RenderData->Sections.size(), "Invalid Section Index while accessing sections");
			return m_RenderData->Sections[SectionIndex];
		}

		/* Returns the material at the given index */
		inline Ref<Material> GetMaterial(uint32_t MaterialIndex = 0) const
		{ 
			GX_ENGINE_ASSERT(MaterialIndex < m_Materials.size(), "Invalid Material Index while accessing materials");
			return m_Materials[MaterialIndex];
		}

		/* Returns the current override material */
		inline Ref<Material> GetOverrideMaterial() const { return m_OverrideMaterial; }

		/* Sets a new override material for the mesh */
		inline void SetOverrideMaterial(const Ref<Material>& NewMat) { m_OverrideMaterial = NewMat; }

		/* Returns the model matrix for the mesh */
		inline const GM::Matrix4& GetModelMatrix() const { return m_Model; }

		/* Returns the original bounds of the mesh */
		inline const GM::BoxBounds& GetBounds() const { return m_Bounds; }
		
		/* Returns the bounding collision box */
		inline const Ref<struct GM::BoundingBox>& GetBoundingBox() const { return m_BoundingBox; }

		/* Sets new state for updating the model matrix */
		inline void UpdateModelMatrix(bool bCalculateMatrix) { m_UpdateModelMatrix = bCalculateMatrix; }

		/* Returns the raw mesh data (Should be modified only while building the mesh */
		inline RawMeshData* GetRawData() const { return m_RawData.get(); }

		/* Returns the material map, mapping the material indices to the section using those materials */
		inline const MaterialToSectionMap& GetMaterialMap() const { return m_MaterialMap; }

		/* Returns whether the mesh resources have been initialised */
		inline bool IsInitialised() const { return m_Initialised; }

		virtual ~Mesh3D();

		/* Adds a new material for the mesh. Returns the material Index */
		uint32_t AddMaterial(const Ref<Material>& InMat);

		/* Intialises all the resource required by renderer (Called by the render thread, in this case - Main thread) */
		bool InitResources();

		/* Releases all the resource required by renderer (Called by the render thread, in this case - Main thread) */
		bool ReleaseResources();

	private:
		/* Builds the mesh from the Raw mesh data (If not already built) */
		void BuildMesh();

		/* Calculate the bounding box and origin bounds of the mesh */
		void CalculateBounds();

	public:
		/* Position of the object */
		GM::Vector3 Position;

		/* Rotation of the object */
		GM::Rotator Rotation;

		/* Scale of the object */
		GM::Vector3 Scale;

		/* Whether to show the details UI window (ImGUI) or not */
		bool bShowDetails : 1;

	protected:
		/* Raw Data of mesh */
		Scope<RawMeshData> m_RawData;

		/* Data required for rendering the mesh properly */
		Scope<RenderDataMesh3D> m_RenderData;

		/* Material used to render the mesh */
		std::vector<Ref<Material>> m_Materials;

		/* Material to override all the materials of the mesh (including all its sections) */
		Ref<Material> m_OverrideMaterial = nullptr;

		/* Model matrix for the mesh */
		GM::Matrix4 m_Model;

		/* Mapping of the materials to the sections using those materials */
		MaterialToSectionMap m_MaterialMap;

		/* Bounds of the box */
		GM::BoxBounds m_Bounds;

		/* AABB containing the whole object (independent of transformation) */
		Ref<GM::BoundingBox> m_BoundingBox;

		/* Whether the mesh needs to updated or not */
		bool m_UpdateModelMatrix;

		/* Whether the mesh resources has been intialised */
		bool m_Initialised = false;
	};
}