#include "CommandController.h"

CommandController::CommandController()
{
	m_commandQueue = nullptr;
	m_commandAllocator = nullptr;
	m_commandList = nullptr;
}

CommandController::~CommandController()
{
}

void CommandController::Create(ID3D12Device* _device)
{
	// Describe and create the command queue.
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	M_ASSERT(SUCCEEDED(_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue))),
		"Failed to created command queue");

	M_ASSERT(SUCCEEDED(_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_commandAllocator))),
		"Failed to create command allocator.");

	M_ASSERT(SUCCEEDED(_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_commandAllocator.Get(), nullptr,
		IID_PPV_ARGS(&m_commandList))),
		"Failed to create command list.");

	// Command lists are created in the recording state, but there is nothing
	// to record yet. The main loop expects it to be closed, so close it now.
	M_ASSERT(SUCCEEDED(m_commandList->Close()), "Failed to close command list.");
}









void CommandController::Reset(ID3D12PipelineState* _pipelineState)
{
	M_ASSERT(SUCCEEDED(m_commandAllocator->Reset()), "Failed to reset command allocator.");
	M_ASSERT(SUCCEEDED(m_commandList->Reset(m_commandAllocator.Get(), _pipelineState)),
		"Failed to reset command list.");
}

void CommandController::Close()
{
	M_ASSERT(SUCCEEDED(m_commandList->Close()), "Failed to close command list.");
}

void CommandController::Execute()
{
	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}

void CommandController::AddBarrier(ID3D12Resource* _resource, D3D12_RESOURCE_STATES _before,
	D3D12_RESOURCE_STATES _after)
{
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(_resource, _before, _after);
	m_commandList->ResourceBarrier(1, &barrier);
}