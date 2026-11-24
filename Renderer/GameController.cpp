#include "GameController.h"
#include "Renderer.h"
#include "ToolsController.h"
#include "Unit.h"
#include "ShaderPhong.h"
#include "ShaderColor.h"
#include "ShaderSkybox.h"

GameController::GameController()
{
    m_windowC = nullptr;
    m_renderer = nullptr;
    // m_camera = nullptr;
    m_toolsC = nullptr;
}

GameController::~GameController()
{
}

void GameController::Initialize(HINSTANCE _hInstance, int _nShowCmd)
{
    m_windowC = &WindowController::GetInstance();
    m_windowC->Create(_hInstance, _nShowCmd, WindowMode::WINDOWED);
    m_renderer = &Renderer::GetInstance();
    m_renderer->ConfigurePipeline(false, m_windowC->GetHWND(), m_windowC->GetWindowWidth(), m_windowC->GetWindowHeight());
    FontController::GetInstance().Initialize(m_renderer->GetDevice().Get(), m_renderer->GetCommands(), m_renderer->GetRenderTargets(), Renderer::GetFrameCount());
    
    Camera::SetCurrent(new Camera(m_windowC->GetWindowWidth(), m_windowC->GetWindowHeight())); // Create the camera
    m_toolsC = &ToolsController::GetInstance();
    m_toolsC->LoadToolsLibrary();
    srand(0); // Seed the random number generator with a fixed value for reproducibility
}

float GameController::RndF(float _min, float _max)
{
    static std::random_device rd; // Provides a non-deterministic seed source
    static std::mt19937 engine(rd()); // Standard Mersenne Twister 19937 generator
    static std::uniform_real_distribution<float> distribution(_min, _max);

    return distribution(engine);
}

double GameController::GetTime()
{
    static LARGE_INTEGER frequency;
    static BOOL useHighRes = QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    return (double)(currentTime.QuadPart) / (double)frequency.QuadPart;
}












void GameController::Run()
{
    m_toolsC->ShowForm();

    vector<IRenderable*> units;
    ShaderColor shaderColor = ShaderColor();
    shaderColor.Initialize();
    ShaderPhong shaderPhong = ShaderPhong();
    shaderPhong.Initialize();
    ShaderSkybox shaderSkybox = ShaderSkybox();
    shaderSkybox.Initialize();

    Unit* light = new Unit();
    light->Initialize("..\\Assets\\Models\\Teapot.obj", &shaderColor);
    light->SetPosition(XMFLOAT3(0.0f, 0.5f, -2.5f));
    light->SetScale(XMFLOAT3(0.005f, 0.005f, 0.005f));
    units.push_back(light);

    Unit* unit = new Unit();
    unit->Initialize("..\\Assets\\Models\\Teapot.obj", &shaderPhong);
    unit->SetPosition({ 0.0f, 0.0f, 0.0f });
    unit->SetScale({ 0.04f, 0.04f, 0.04f });
    units.push_back(unit);

    //Main message loop
    MSG msg = {};
    bool running = true;
    double lastTime = GetTime();
    int fps = 0;
    wstring fpsS = L"0";




    while (running)
    {
        double currentTime = GetTime();
        fps++;
        if (currentTime - lastTime >= 1)
        {
            fpsS = L"FPS: " + to_wstring(fps);
            fps = 0;
            lastTime = currentTime;
        }

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) running = false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!running) break;

        m_renderer->GetBGColor()[0] = (float)m_toolsC->GetRedData();
        m_renderer->GetBGColor()[1] = (float)m_toolsC->GetGreenData();
        m_renderer->GetBGColor()[2] = (float)m_toolsC->GetBlueData();

        Unit* unit = static_cast<Unit*>(units[1]);
        auto pos = unit->GetPosition();
        auto rot = unit->GetRotation();
        auto scl = unit->GetScale();

        if (m_windowC->GetKeyStates()[VK_RIGHT]) pos.x += 0.01f;
        if (m_windowC->GetKeyStates()[VK_LEFT]) pos.x -= 0.01f;
        if (m_windowC->GetKeyStates()[VK_UP]) pos.y += 0.01f;
        if (m_windowC->GetKeyStates()[VK_DOWN]) pos.y -= 0.01f;

        if (m_windowC->GetKeyStates()['Q']) rot.y += 0.1f;
        if (m_windowC->GetKeyStates()['W']) rot.y -= 0.1f;
        if (m_windowC->GetKeyStates()['A']) scl.x -= 0.1f;
        if (m_windowC->GetKeyStates()['S']) scl.x += 0.1f;

        unit->SetPosition(XMFLOAT3(pos.x, pos.y, pos.z));
        unit->SetRotation(XMFLOAT3(rot.x, rot.y, rot.z));
        unit->SetScale(XMFLOAT3(scl.x, scl.y, scl.z));

        FontController::GetInstance().Write(fpsS, L"Arial_20", D2D1_RECT_F(20, 80, 800, 120), D2D1::ColorF::Yellow);
        shaderColor.ResetCurrentObjectIndex();
        shaderPhong.ResetCurrentObjectIndex();
        shaderSkybox.ResetCurrentObjectIndex();

        m_renderer->Render(units, Camera::Current()->GetViewProjection());
    }

    for (auto u : units)
    {
        delete u;
    }
    units.clear();

    delete Camera::Current();
    Camera::SetCurrent(nullptr);
}

