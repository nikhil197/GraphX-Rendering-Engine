#pragma once

namespace GraphX
{
	class RendererAsset
	{
	protected:
		uint32_t m_RendererID = 0;

	public:
		RendererAsset() = default;

		uint32_t GetID() const { return m_RendererID; }

		bool operator==(const RendererAsset& Other) const
		{
			return m_RendererID == Other.m_RendererID;
		}

		static bool Equals(const Ref<RendererAsset>& Asset1, const Ref<RendererAsset>& Asset2)
		{
			return Asset1->m_RendererID == Asset2->m_RendererID;
		}
	};
}