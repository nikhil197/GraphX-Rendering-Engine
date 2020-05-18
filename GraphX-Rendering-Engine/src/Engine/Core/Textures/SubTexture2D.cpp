#include "pch.h"

#include "SubTexture2D.h"
#include "Engine/Core/Textures/Texture2D.h"

namespace GraphX
{
	SubTexture2D::SubTexture2D(const Ref<Texture2D>& InTexture, const GM::Vector2& Min, const GM::Vector2& Max)
		: m_Texture(InTexture)
	{
		m_TexCoords[0].x = Min.x; m_TexCoords[0].y = Min.y;
		m_TexCoords[1].x = Max.x; m_TexCoords[1].y = Min.y;
		m_TexCoords[2].x = Max.x; m_TexCoords[2].y = Max.y;
		m_TexCoords[3].x = Min.x; m_TexCoords[3].y = Max.y;
	}

	Ref<SubTexture2D> SubTexture2D::Create(const Ref<Texture2D>& InTexture, const GM::Vector2& CellIndex, const GM::Vector2& CellSize, const GM::Vector2& CellSpan, bool FromBottomLeft)
	{
		GM::Vector2 Min, Max;
		if (FromBottomLeft)
		{
			Min.x = (CellIndex.x * CellSize.x) / InTexture->GetWidth(); Min.y = (CellIndex.y * CellSize.y) / InTexture->GetHeight();
			Max.x = ((CellIndex.x + CellSpan.x) * CellSize.x) / InTexture->GetWidth(); Max.y = ((CellIndex.y + CellSpan.y) * CellSize.y) / InTexture->GetHeight();
		}
		else
		{
			Min.x = (CellIndex.x * CellSize.x) / InTexture->GetWidth(); Min.y = 1.0f - ((CellIndex.y + CellSpan.y) * CellSize.y) / InTexture->GetHeight();
			Max.x = ((CellIndex.x + CellSpan.x) * CellSize.x) / InTexture->GetWidth(); Max.y = 1.0f - (CellIndex.y * CellSize.y) / InTexture->GetHeight();
		}
		
		return CreateRef<SubTexture2D>(InTexture, Min, Max);
	}
}