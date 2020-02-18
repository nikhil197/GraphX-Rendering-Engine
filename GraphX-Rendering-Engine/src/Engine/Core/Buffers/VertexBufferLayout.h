#pragma once

namespace GraphX
{
	/* Data types for the buffers (To Decouple Engine Implementation from specific API Implementations) */
	enum class BufferDataType
	{
		None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, UInt, Bool, Mat3, Mat4
	};

	/* Returns the size of the Buffer Data Type */
	static unsigned int GetBufferDataTypeSize(BufferDataType Type)
	{
		switch (Type)
		{
			case BufferDataType::Float:		return 4;
			case BufferDataType::Float2:	return 4 * 2;
			case BufferDataType::Float3:	return 4 * 3;
			case BufferDataType::Float4:	return 4 * 4;
			case BufferDataType::Int:		return 4;
			case BufferDataType::Int2:		return 4 * 2;
			case BufferDataType::Int3:		return 4 * 3;
			case BufferDataType::Int4:		return 4 * 4;
			case BufferDataType::UInt:		return 4;
			case BufferDataType::Bool:		return 1;
			case BufferDataType::Mat3:		return 4 * 3 * 3;
			case BufferDataType::Mat4:		return 4 * 4 * 4;
		}

		GX_ENGINE_ASSERT(false, "Unknown Buffer Data Type");
		return 0;
	}

	struct BufferLayoutElement
	{
		BufferDataType Type;
		unsigned int Size;
		unsigned int Offset;
		bool Normalised;

		BufferLayoutElement(BufferDataType type, bool normalised = false)
			: Type(type), Size(GetBufferDataTypeSize(type)), Offset(0), Normalised(normalised)
		{
		}

		int GetComponentCount() const
		{
			switch (Type)
			{
				case BufferDataType::Float:		return 1;
				case BufferDataType::Float2:	return 2;
				case BufferDataType::Float3:	return 3;
				case BufferDataType::Float4:	return 4;
				case BufferDataType::Int:		return 1;
				case BufferDataType::Int2:		return 2;
				case BufferDataType::Int3:		return 3;
				case BufferDataType::Int4:		return 4;
				case BufferDataType::UInt:		return 1;
				case BufferDataType::Bool:		return 1;
				case BufferDataType::Mat3:		return 3 * 3;
				case BufferDataType::Mat4:		return 4 * 4;
			}

			GX_ENGINE_ASSERT(false, "Unknown Buffer Data Type");
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
		VertexBufferLayout(const std::initializer_list<BufferLayoutElement>& Elements)
			: m_Elements(Elements), m_Stride(0)
		{
			CalculateStrideAndOffset();
		}

		void CalculateStrideAndOffset()
		{
			for (BufferLayoutElement& Element : m_Elements)
			{
				Element.Offset = m_Stride;
				m_Stride += Element.Size;	
			}
		}

		/* Returns the layout elements specified for this buffer */
		const std::vector<BufferLayoutElement>& GetElements() const { return m_Elements; }

		/* Returns the current stride */
		unsigned int GetStride() const { return m_Stride; }
	};
}