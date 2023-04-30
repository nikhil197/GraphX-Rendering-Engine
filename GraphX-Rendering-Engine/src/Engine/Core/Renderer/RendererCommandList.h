#pragma once

#include "Renderer.h"

namespace GraphX
{
	class RenderCommandBase
	{
	public:
		virtual void ExecuteAndDestruct() = 0;
	};

	template<typename Cmd>
	class RenderCommand : RenderCommandBase
	{
	public:
		virtual void ExecuteAndDestruct() override final
		{
			Cmd* ThisCmd = static_cast<Cmd*>(this);
			ThisCmd->Execute();
			ThisCmd->~Cmd();
		}
	};

	template<typename LAMBDA>
	class RenderLambdaCommand : RenderCommandBase
	{
	private:
		LAMBDA m_Lambda;

	public: 
		RenderLambdaCommand(LAMBDA&& lambda)
			: m_Lambda(std::forward<LAMBDA>(lambda)) 
		{}

		virtual void ExecuteAndDestruct() override final
		{
			m_Lambda();
			m_Lambda.~m_Lambda();
		}
	};

	template<typename TCmd>
	void EnqueueRenderCommand(TCmd* Cmd)
	{
		RenderCommandBase* BaseCmd = static_cast<RenderCommandBase*>(Cmd);
		GX_ENGINE_ASSERT(BaseCmd != nullptr, "CommandList: Invalid CommandType");
		Renderer::SubmitRenderCommand(Cmd);
	}

	template<typename LAMBDA>
	void EnqueueLambdaRenderCommand(LAMBDA&& lambda)
	{
		if (IsInRenderingThread())
		{
			lambda();
		}
		else
		{
			EnqueueRenderCommand<RenderLambdaCommand>(new RenderLambdaCommand<LAMBDA>(std::forward<LAMBDA>(lambda)));
		}
	}

#define RENDER_COMMAND(CommandName)												\
	struct CommandName##RenderCommand : public RenderCommandBase {				\
			const std::string GetName() const { return #CommandName; }

	class RendererCommandList
	{
		friend class RenderThread;
	public:
		RendererCommandList() = default; // For now this constructor is default, but might be changed in the future

		~RendererCommandList();

		void Push(RenderCommandBase* command)
		{
			m_CommandsForNextPass.emplace_back(command);
		}

		template<typename T, typename ... Args>
		void Push(Args&& ... args)
		{
			m_CommandsForNextPass.emplace_back(new T(std::forward<Args>(args)...));
		}

	private:
		// Called at the start of a pass to get the new commands to execute
		void SwapCommandBuffers();

	private:

		std::mutex m_CommandListMutex;

		// Render Commands to be executed in the next render pass
		std::deque<RenderCommandBase*> m_CommandsForNextPass;

		// Render commands being executed in the current render pass
		std::deque<RenderCommandBase*> m_CommandsForCurrentPass;
	};
}