#pragma once

namespace GraphX
{
	struct BufferLayoutElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalised;

		static int GetSizeOfType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT:			return sizeof(GLfloat);
			case GL_UNSIGNED_INT:	return sizeof(GLuint);
			case GL_UNSIGNED_BYTE:	return sizeof(GLubyte);
			}

			// couldn't identify the type
			GX_ASSERT(false, "Unknown Type");
			return 0;
		}

	};

	class VertexBufferLayout
	{
	private:
		/* Collections of the layouts for all the vertex attibutes */
		std::vector<BufferLayoutElement> m_Elements;

		/* Number of bytes between two vertices (size of a vertex) */
		unsigned int m_Stride;

	public:
		// Initialise the Buffer Layout object
		VertexBufferLayout()
			: m_Stride(0) { }

		// Invalid type 
		// Here count is the number of components in the vertex attributes (like 2 in a vec2 or 3 in a vec3)
		template<typename T>
		void Push(unsigned int count)
		{
			static_assert(false);
		}

		// Template specializations for various OpenGL types
		template<>
		void Push<float>(unsigned int count)
		{
			m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });

			// increase the stride for the next vertex attribute
			m_Stride += count * BufferLayoutElement::GetSizeOfType(GL_FLOAT);
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			m_Stride += count * BufferLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
		}

		/* Returns the layout elements specified for this buffer */
		const std::vector<BufferLayoutElement>& GetElements() const { return m_Elements; }

		/* Returns the current stride */
		unsigned int GetStride() const { return m_Stride; }
	};
}