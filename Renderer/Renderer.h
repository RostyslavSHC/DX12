#pragma once

#include "StandardIncludes.h"

class Renderer : public Singleton<Renderer>
{
public:
    // Constructors / Destructors
    Renderer();
    virtual ~Renderer();

    // Accessors
    ComPtr<ID3D12Device> GetDevice() { return m_device; } // Return the D3D12 device

    // Methods
    void ConfigurePipeline(bool _userWarpDevice); // Configure the pipeline

private:
    // Methods
    UINT EnableDebugLayer(); // Enable the debug layer
    void CreateDevice(bool _userWarpDevice); // Create the D3D12 device
    void GetHardwareAdapter(_In_ IDXGIFactory1* _factory, _Outptr_result_maybenull_ IDXGIAdapter1** _adapter, bool reqHighPerfAdapter = false); // Get the hardware adapter

    // Members
    ComPtr<IDXGIFactory4> m_factory; // The DXGI factory
    ComPtr<ID3D12Device> m_device; // The D3D12 device
};