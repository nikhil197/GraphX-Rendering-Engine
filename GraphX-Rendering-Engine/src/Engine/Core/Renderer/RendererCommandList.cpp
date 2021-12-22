#include "pch.h"
#include "RendererCommandList.h"

namespace GraphX
{
	void RendererCommandList::SwapCommandBuffers()
	{
		std::lock_guard<std::mutex> guard(m_CommandListMutex);

		m_CommandsForCurrentPass = m_CommandsForNextPass;
		m_CommandsForNextPass = std::deque<RenderCommandBase*>();
	}

	RendererCommandList::~RendererCommandList()
	{
		// The executor will have to make sure that both the lists are emtpy
	}
}