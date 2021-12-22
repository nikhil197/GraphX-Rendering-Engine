#pragma once

namespace GraphX
{
	class RendererResource
	{
	protected:
		uint32_t m_RendererID = 0;

	public:
		RendererResource() = default;

		uint32_t GetID() const { return m_RendererID; }

		virtual void InitRHI() = 0;

		virtual void ReleaseRHI() = 0;

		bool operator==(const RendererResource& Other) const
		{
			return m_RendererID == Other.m_RendererID;
		}

		static bool Equals(const Ref<RendererResource>& Asset1, const Ref<RendererResource>& Asset2)
		{
			return Asset1->m_RendererID == Asset2->m_RendererID;
		}
	};
}