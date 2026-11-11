#include "Renderer.h"
#include "Shader.h"

// Default constructor
Renderer::Renderer()
{
    // Set default values so there are no garbage values
    m_factory = nullptr;
    m_device = nullptr;
    m_swapChain = nullptr;
    m_rtvHeap = nullptr;

    for (int count = 0; count < m_frameCount; count++)
    {
        m_renderTargets[count] = nullptr;
    }

    m_fence = nullptr;
    m_fenceEvent = nullptr;
    m_fenceValue = 0;
    m_frameIndex = 0;
    m_rtvDescriptorSize = 0;
    m_initialized = false;
    memset(m_bgColor, 0, sizeof(float) * 4);
    m_viewport = {};
    m_scissorRect = {};
    m_commands = nullptr;
}

// Destructor
Renderer::~Renderer()
{
    Destroy();
}

// Configure the pipeline. Pipeline is a series of stages that are executed in order
void Renderer::ConfigurePipeline(bool _userWarpDevice, HWND _hwnd, int _wWidth, int _wHeight)
{
    UINT dxgiFactoryFlags = EnableDebugLayer(); // Enable the debug layer
    M_ASSERT(SUCCEEDED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_factory))), "Create DXGI factory failed"); // If it fails, show error and stop
    CreateDevice(_userWarpDevice); // Create the device

    m_commands = new CommandController();
    m_commands->Create(m_device.Get());

    CreateSwapChain(_hwnd, _wWidth, _wHeight); // Create the swap chain
    CreateRenderTargetView(); // Create the render target view
    CreateFence(); // Create the fence
    CreateViewport(_wWidth, _wHeight);

    // Do not support fullscreen transitions yet
    m_factory->MakeWindowAssociation(_hwnd, DXGI_MWA_NO_ALT_ENTER);
    m_initialized = true;
}

void Renderer::CreateViewport(int _width, int _wHeight)
{
    m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(_width), static_cast<float>(_wHeight));
    m_scissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(_width), static_cast<LONG>(_wHeight));
}

// Enable the debug layer
UINT Renderer::EnableDebugLayer()
{
    UINT dxgiFactoryFlags = 0; // Start with no flags

    // If the debug layer is available
#if defined(_DEBUG)
    ComPtr<ID3D12Debug> debugController; // Create a debug interface
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) // Get the debug interface
    {
        debugController->EnableDebugLayer();
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG; // Enable additional debug layers.
    }
#endif

    return dxgiFactoryFlags; // Return the flags
}

// Create the device
void Renderer::CreateDevice(bool _userWarpDevice)
{
    if (_userWarpDevice) // If the user wants to use the warp (software) device
    {
        ComPtr<IDXGIAdapter> warpAdapter; // Create a warp adapter
        M_ASSERT(SUCCEEDED(m_factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter))), "Enum warp adapter failed"); // Get the warp adapter and make sure it works
        M_ASSERT(SUCCEEDED(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device))), "Create warp device failed"); // Create the warp device and make sure it works
    }
    else // If the user wants to use the hardware device
    {
        ComPtr<IDXGIAdapter1> hardwareAdapter; // Create a hardware adapter
        GetHardwareAdapter(m_factory.Get(), &hardwareAdapter); // Get the hardware adapter
        M_ASSERT(SUCCEEDED(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device))), "Credte hardware device failed"); // Create the hardware device and make sure it works
    }

    ComPtr<ID3D12InfoQueue> infoQueue;
    if (SUCCEEDED(m_device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
    {
        D3D12_MESSAGE_ID hide[] = { D3D12_MESSAGE_ID_CREATERESOURCE_STATE_IGNORED };
        D3D12_INFO_QUEUE_FILTER filter = {};
        filter.DenyList.NumIDs = _countof(hide);
        filter.DenyList.pIDList = hide;
        infoQueue->AddStorageFilterEntries(&filter);
    }



}

// Get the hardware adapter
void Renderer::GetHardwareAdapter(_In_ IDXGIFactory1* _factory, _Outptr_result_maybenull_ IDXGIAdapter1** _adapter, bool _reqHighPerfAdapter)
{
    *_adapter = nullptr; // Start with no adapter

    // If the factory supports IDXGIFactory6
    ComPtr<IDXGIAdapter1> adapter;
    ComPtr<IDXGIFactory6> factory6;

    // If the factory supports IDXGIFactory6, try to get a high performance adapter
    if (SUCCEEDED(_factory->QueryInterface(IID_PPV_ARGS(&factory6))))
    {
        DXGI_GPU_PREFERENCE gpuPref = DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE;

        if (!_reqHighPerfAdapter) gpuPref = DXGI_GPU_PREFERENCE_UNSPECIFIED; // FIXED BUG HERE, should be !_reqHighPerfAdapter

        // Try to get a high performance adapter, if it fails, get any adapter
        for (UINT adapterIndex = 0;
            SUCCEEDED(factory6->EnumAdapterByGpuPreference(
                adapterIndex,
                gpuPref,
                IID_PPV_ARGS(&adapter)));
            ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue; // Skip software adapters for now

            if (SUCCEEDED(D3D12CreateDevice(
                adapter.Get(),
                D3D_FEATURE_LEVEL_11_0,
                __uuidof(ID3D12Device),
                nullptr))) break; // If it works, break
        }
    }

    // If the factory doesn't support IDXGIFactory6, try to get any adapter
    if (adapter.Get() == nullptr)
    {
        // Try to get any adapter
        for (UINT adapterIndex = 0; SUCCEEDED(_factory->EnumAdapters1(adapterIndex, &adapter)); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue; // Skip software adapters

            if (SUCCEEDED(D3D12CreateDevice(
                adapter.Get(),
                D3D_FEATURE_LEVEL_11_0,
                __uuidof(ID3D12Device),
                nullptr))) break; // If it works, break
        }
    }

    *_adapter = adapter.Detach(); // Return the adapter (if it exists)
}

// Create the swap chain
void Renderer::CreateSwapChain(HWND _hwnd, int _wWidth, int _wHeight)
{
    // Describe and create the swap chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {}; // Describe the swap chain
    swapChainDesc.BufferCount = m_frameCount;
    swapChainDesc.Width = _wWidth;
    swapChainDesc.Height = _wHeight;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // Use 32-bit color
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // The back buffer will be used as a render target
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // The back buffer will be discarded
    swapChainDesc.SampleDesc.Count = 1;

    ComPtr<IDXGISwapChain1> swapChain;

    M_ASSERT(SUCCEEDED(m_factory->CreateSwapChainForHwnd(
        m_commands->GetCommandQueue().Get(),
        _hwnd,
        &swapChainDesc,
        nullptr,
        nullptr,
        &swapChain)),
        "Failed to create swap chain");

    M_ASSERT(SUCCEEDED(swapChain.As(&m_swapChain)), "Casting swapchain failed");
    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex(); // Get the current back buffer index
}

// Create the render target view
void Renderer::CreateRenderTargetView()
{
    // Describe and create a render target view (RTV) descriptor heap
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {}; // Describe the RTV descriptor heap
    rtvHeapDesc.NumDescriptors = m_frameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    M_ASSERT(SUCCEEDED(m_device->CreateDescriptorHeap(
        &rtvHeapDesc,
        IID_PPV_ARGS(&m_rtvHeap))),
        "Failed to create descriptor heap"); // If it fails, show error

    m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV); // Get the size of the RTV descriptor

    // Create a RTV for each frame
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart()); // Get the start of the RTV descriptor heap

    // Create a RTV for each back buffer
    for (UINT n = 0; n < m_frameCount; n++)
    {
        M_ASSERT(SUCCEEDED(m_swapChain->GetBuffer(
            n,
            IID_PPV_ARGS(&m_renderTargets[n]))),
            "Swapchain get back buffer failed."); // Get the back buffer

        m_device->CreateRenderTargetView(
            m_renderTargets[n].Get(),
            nullptr,
            rtvHandle); // Create the RTV for the back buffer and add it to the RTV descriptor heap

        rtvHandle.Offset(1, m_rtvDescriptorSize); // Move to the next descriptor
    }
}

// Create the fence
void Renderer::CreateFence()
{
    M_ASSERT(SUCCEEDED(m_device->CreateFence(
        0,
        D3D12_FENCE_FLAG_NONE,
        IID_PPV_ARGS(&m_fence))),
        "Failed to create fence."); // Create the fence and make sure it works

    m_fenceValue = 1; // The fence starts at 1

    // Create an event handle to use for frame synchronization
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr); // Create the event
    M_ASSERT(m_fenceEvent != nullptr, "Failed to create fence event."); // If it fails, show error
}

// Render
void Renderer::Render(vector<IRenderable*>& _renderables, XMMATRIX _viewProj)
{
    if (!m_initialized) return;

    m_commands->Reset();
    m_commands->AddBarrier(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
    auto c = m_commands->GetCommandList().Get();
    c->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    c->RSSetViewports(1, &m_viewport);
    c->RSSetScissorRects(1, &m_scissorRect);
    c->ClearRenderTargetView(rtvHandle, m_bgColor, 0, nullptr);
    for (auto& renderable : _renderables)
    {
        renderable->GetShader()->SetPipelineState();
        renderable->GetShader()->BindObjectCB();
        renderable->Render(_viewProj);
		renderable->GetShader()->IncrementCurrentObjectIndex();
    }

    m_commands->Close();
    m_commands->Execute();
    m_commands->GetCommandList()->Reset(m_commands->GetCommandAllocator().Get(), nullptr);
    // FontController::GetInstance().Write(L"Segoe UI 32", D2D1_RECT_F{ 20, 20, 800, 60 }, D2D1::ColorF::Yellow, m_frameIndex);
    // FontController::GetInstance().Write(L"Arial 20", D2D1_RECT_F{ 20, 80, 800, 120 }, D2D1::ColorF::White, m_frameIndex);
    FontController::GetInstance().Write(L"Segoe UI_32", D2D1_RECT_F{ 20, 20, 800, 60 }, D2D1::ColorF::Yellow, m_frameIndex);
    FontController::GetInstance().Write(L"Arial_20", D2D1_RECT_F{ 20, 80, 800, 120 }, D2D1::ColorF::White, m_frameIndex);
    m_commands->AddBarrier(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_commands->Close();
    m_commands->Execute();
    M_ASSERT(SUCCEEDED(m_swapChain->Present(1, 0)), "Failed to present swapchain.");

    WaitForPreviousFrame();
}

// Destroy
void Renderer::Destroy()
{
    // Ensure that the GPU is no longer referencing resources that are about to be cleaned up by the destructor
    WaitForPreviousFrame();
    CloseHandle(m_fenceEvent);
    delete m_commands;
}

/*
// Populate the command list
void Renderer::PopulateCommandList(IRenderable* _renderable, XMMATRIX _viewProj)
{
    // Reset only after fence allows
    m_commands->Reset(_renderable->GetShader()->GetPipelineState().Get());

    m_commands->AddBarrier(
        m_renderTargets[m_frameIndex].Get(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
        m_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
        m_frameIndex,
        m_rtvDescriptorSize); // Get the start of the RTV descriptor heap

    auto c = m_commands->GetCommandList().Get();
    c->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);
    c->RSSetViewports(1, &m_viewport); // Set the viewport
    c->RSSetScissorRects(1, &m_scissorRect); // Set the scissor rect
    c->ClearRenderTargetView(rtvHandle, m_bgColor, 0, nullptr);
    _renderable->Render(_viewProj);

    // const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f }; // SET THE COLOR OF BACKGROUND -------------------------------------
    // c->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    m_commands->AddBarrier(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
    m_commands->Close();
}
*/

// Wait for the previous frame
void Renderer::WaitForPreviousFrame()
{
    // WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE
    // Signal and increment the fence value
    const UINT64 fence = m_fenceValue;

    M_ASSERT(SUCCEEDED(m_commands->GetCommandQueue().Get()->Signal(m_fence.Get(), fence)), "Failed to signal fence.");

    m_fenceValue++;

    if (m_fence->GetCompletedValue() < fence) // If the fence hasn't been met, wait until it is met
    {
        M_ASSERT(SUCCEEDED(
            m_fence->SetEventOnCompletion(fence, m_fenceEvent)),
            "Failed to set fence complete."); // Set an event on the fence to signal when it's met

        WaitForSingleObject(m_fenceEvent, INFINITE);
    }

    m_frameIndex = m_swapChain->GetCurrentBackBufferIndex(); // Advance the frame index
}