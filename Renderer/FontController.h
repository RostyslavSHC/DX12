#pragma once

#include "StandardIncludes.h"

class FontController : public Singleton<FontController>
{
public:
    // Constructors/Destructors
    FontController();
    virtual ~FontController() {}

    // Methods
    void Initialize(ID3D12Device* _d3d12Device, CommandController* _commandController, ComPtr<ID3D12Resource>* _renderTargets, UINT _frameCount);
    void Write(wstring _font, D2D1_RECT_F _destination, D2D1::ColorF::Enum _color, UINT _frameIndex);

private:
    // Methods
    void CreateD3D11Device(ID3D12Device* _d3d12Device);
    void CreateD2D();
    void CreateRenderTargets();
    void CreateBrushes();
    void CreateFontFace(wstring _fontName, int _dipSize, DWRITE_FONT_WEIGHT _weight);

    // Members
    CommandController* m_commandController;
    ComPtr<ID3D12Resource>* m_renderTargets;
    UINT m_frameCount;
    float m_dpi;
    ComPtr<ID3D11Device> m_d3d11Device;
    ComPtr<ID3D11DeviceContext> m_d3d11Context;
    ComPtr<ID3D11On12Device> m_d3d11On12;
    ComPtr<ID2D1Factory3> m_d2dFactory;
    ComPtr<ID2D1Device2> m_d2dDevice;
    ComPtr<ID2D1DeviceContext2> m_d2dContext;
    ComPtr<IDWriteFactory5> m_dwriteFactory;
    vector<ComPtr<ID3D11Resource>> m_wrappedBuffers;
    vector<ComPtr<ID2D1Bitmap1>> m_d2dTargets;
    map<wstring, ComPtr<IDWriteTextFormat>> m_fonts;
    map<D2D1::ColorF::Enum, ComPtr<ID2D1SolidColorBrush>> m_brushes;
};