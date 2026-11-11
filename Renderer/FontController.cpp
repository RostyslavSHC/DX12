#include "FontController.h"

FontController::FontController()
{
    m_commandController = nullptr;
    m_renderTargets = nullptr;
    m_frameCount = 0;
    m_dpi = 0;
}

void FontController::Initialize(ID3D12Device* _d3d12Device, CommandController* _commandController, ComPtr<ID3D12Resource>* _renderTargets, UINT _frameCount)
{
    m_commandController = _commandController;
    m_renderTargets = _renderTargets;
    m_frameCount = _frameCount;
    CreateD3D11Device(_d3d12Device);
    CreateD2D();
    CreateRenderTargets();
    CreateBrushes();
    CreateFontFace(L"Segoe UI", 32, DWRITE_FONT_WEIGHT_NORMAL);
    CreateFontFace(L"Arial", 20, DWRITE_FONT_WEIGHT_BOLD);
}

void FontController::CreateD3D11Device(ID3D12Device* _d3d12Device)
{
    D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
    M_ASSERT(SUCCEEDED(D3D11On12CreateDevice(_d3d12Device, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
        levels, _countof(levels), (IUnknown**)m_commandController->GetCommandQueue().GetAddressOf(),
        1, 0, &m_d3d11Device, &m_d3d11Context, nullptr)), "Failed to create D3D11On12Device.");
    M_ASSERT(SUCCEEDED(m_d3d11Device.As(&m_d3d11On12)), "Failed to cast device.");
}

void FontController::CreateD2D()
{
    ComPtr<IDXGIDevice> dxgiDevice;
    M_ASSERT(SUCCEEDED(m_d3d11Device.As(&dxgiDevice)), "Failed to cast device.");
    M_ASSERT(SUCCEEDED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_PPV_ARGS(&m_d2dFactory))),
        "Failed to create D2D1 Factory");
    M_ASSERT(SUCCEEDED(m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice)), "Failed to create D2D device.");
    M_ASSERT(SUCCEEDED(m_d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_d2dContext)),
        "Failed to create D2D device context.");

    M_ASSERT(SUCCEEDED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
        (IUnknown**)m_dwriteFactory.GetAddressOf())), "Failed to create DW factory");

    m_dpi = (float)GetDpiForWindow(WindowController::GetInstance().GetHWND());
    m_d2dContext->SetDpi(m_dpi, m_dpi);
}

void FontController::CreateRenderTargets()
{
    m_wrappedBuffers.resize(m_frameCount);
    m_d2dTargets.resize(m_frameCount);
    for (UINT i = 0; i < m_frameCount; ++i)
    {
        D3D11_RESOURCE_FLAGS rf{};
        rf.BindFlags = D3D11_BIND_RENDER_TARGET;

        auto rt = m_renderTargets[i].Get();
        M_ASSERT(SUCCEEDED(m_d3d11On12->CreateWrappedResource(
            (IUnknown*)rt, &rf, D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            IID_PPV_ARGS(&m_wrappedBuffers[i]))), "Failed to create resource.");

        ComPtr<IDXGISurface> surface;
        M_ASSERT(SUCCEEDED(m_wrappedBuffers[i].As(&surface)), "Failed to cast buffer.");

        auto props = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED), m_dpi, m_dpi);
        M_ASSERT(SUCCEEDED(m_d2dContext->CreateBitmapFromDxgiSurface(surface.Get(), &props, &m_d2dTargets[i])),
            "Failed to create bitmap from DXGI surface.");
    }
}

void FontController::CreateBrushes()
{
    ComPtr<ID2D1SolidColorBrush> brush;
    M_ASSERT(SUCCEEDED(m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush)), "Failed to create brush.");
    m_brushes[D2D1::ColorF::White] = brush;
    M_ASSERT(SUCCEEDED(m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &brush)), "Failed to create brush.");
    m_brushes[D2D1::ColorF::Yellow] = brush;

    M_ASSERT(SUCCEEDED(m_d2dContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brush)), "Failed to create brush.");
    m_brushes[D2D1::ColorF::Red] = brush;
}

void FontController::CreateFontFace(wstring _fontName, int _dipSize, DWRITE_FONT_WEIGHT _weight)
{
    ComPtr<IDWriteTextFormat> font;
    M_ASSERT(SUCCEEDED(m_dwriteFactory->CreateTextFormat(_fontName.c_str(), nullptr,
        _weight, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
        (float)_dipSize, L"en-us", &font)), "Failed to create font face.");

    M_ASSERT(SUCCEEDED(font->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING)), "Failed to set font text alignment.");
    M_ASSERT(SUCCEEDED(font->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)), "Failed to set paragraph alignment.");
    wstring key = _fontName + L"_" + to_wstring(_dipSize);
    m_fonts[key] = font;
}

void FontController::Write(wstring _font, D2D1_RECT_F _destination, D2D1::ColorF::Enum _color, UINT _frameIndex)
{
    M_ASSERT(m_brushes.contains(_color), "Brush color not instantiated.");

    ID3D11Resource* res[] = { m_wrappedBuffers[_frameIndex].Get() };
    m_d3d11On12->AcquireWrappedResources(res, 1);

    m_d2dContext->SetTarget(m_d2dTargets[_frameIndex].Get());
    m_d2dContext->BeginDraw();
    m_d2dContext->DrawTextW(L"Hello DX12 + DirectWrite", 25, m_fonts[_font].Get(),
        _destination, m_brushes[_color].Get());
    M_ASSERT(SUCCEEDED(m_d2dContext->EndDraw()), "Failed to draw text");

    m_d3d11On12->ReleaseWrappedResources(res, 1);
    m_d3d11Context->Flush();
}
