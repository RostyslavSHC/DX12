#include "Renderer.h"

// Default constructor
Renderer::Renderer()
{
    // Set default values so there are no garbage values
    m_factory = nullptr;
    m_device = nullptr;
}

// Destructor
Renderer::~Renderer()
{
}

// Configure the pipeline. Pipeline is a series of stages that are executed in order
void Renderer::ConfigurePipeline(bool _userWarpDevice)
{
    UINT dxgiFactoryFlags = EnableDebugLayer(); // Enable the debug layer
    M_ASSERT(SUCCEEDED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_factory))), "Create DXGI factory failed"); // If it fails, show error and stop
    CreateDevice(_userWarpDevice); // Create the device
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
            SUCCEEDED(factory6->EnumAdapterByGpuPreference(adapterIndex,
                gpuPref,
                IID_PPV_ARGS(&adapter)));
            ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue; // Skip software adapters for now
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))) break; // If it works, break
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
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))) break; // If it works, break
        }
    }

    *_adapter = adapter.Detach(); // Return the adapter (if it exists)
}