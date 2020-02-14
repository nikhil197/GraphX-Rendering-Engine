#pragma once

#include "Textures/Texture2D.h"

namespace std
{
	template<>
	struct hash<reference_wrapper<GraphX::Texture2D>>
	{
		std::size_t operator()(reference_wrapper<GraphX::Texture> const& Tex) const noexcept
		{
			std::size_t h1(std::hash<std::string>{} (Tex.get().GetFilePath()));
			std::size_t h2(std::hash<int>{} (Tex.get().GetWidth()));
			std::size_t h3(std::hash<int>{} (Tex.get().GetHeight()));

			return h1 ^ (h2 << 1) ^ (h3 << 2);
		}
	};
}