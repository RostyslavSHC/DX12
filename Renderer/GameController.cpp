#include "GameController.h"
#include "Renderer.h"
#include "ToolsController.h"
#include "Unit.h"
#include "ShaderPhong.h"
#include "ShaderColor.h"
#include "ShaderSkybox.h"
#include "ShaderSolidColor.h"
#include "NPC.h"

GameController::GameController()
{
    m_windowC = nullptr;
    m_renderer = nullptr;
    // m_camera = nullptr;
    m_toolsC = nullptr;

    m_shaderSolidColor = nullptr;
    m_player = nullptr;
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

    FontController::GetInstance().Initialize(
        m_renderer->GetDevice().Get(),
        m_renderer->GetCommands(),
        m_renderer->GetRenderTargets(),
        Renderer::GetFrameCount());

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

void GameController::SceneInitialize()
{
    // Initialize shaders
    m_shaderSolidColor = new ShaderSolidColor();
    m_shaderSolidColor->Initialize();
    m_shaderSolidColor->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
    // m_shaderSolidColor->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });

    // Configure the camera
    Camera::Current()->SetView(
        { 0.0f, 0.0f, -40.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f });

    // Initialize the player
    m_player = new Unit();
    m_player->Initialize("..\\Assets\\Models\\Cube.obj", m_shaderSolidColor);
    m_player->SetPosition({ 0.0f, 0.0f, 0.0f });
    // m_player->SetScale({ 2.0f, 2.0f, 2.0f });
    m_player->SetScale({ 2.5f, 2.5f, 2.5f });

    m_units.push_back(m_player);

    InitializeNPCs(); // Initialize NPCs
}



void GameController::InitializeNPCs()
{
    const int npcCount = 8;
    const float radius = 10.0f;

    for (int i = 0; i < npcCount; i++)
    {
        // Calculate the position around the player
        float angle = XM_2PI * i / npcCount;

        float x = cosf(angle) * radius;
        float y = sinf(angle) * radius;

        // Initialize the NPC
        NPC* npc = new NPC();
        npc->Initialize("..\\Assets\\Models\\Cube.obj", m_shaderSolidColor);

        npc->SetPosition({ x, y, 0.0f });
        npc->SetScale({ 2.5f, 2.5f, 2.5f });

        m_npcs.push_back(npc);
        m_units.push_back(npc);
    }
}










bool GameController::ProcessMessages()
{
    MSG msg = {};

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
            return false;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}


void GameController::HandleInput(float _deltaTime)
{
    auto pos = m_player->GetPosition();
    XMFLOAT3 previousPosition = pos;

    float speed = 10.0f;
    float movement = speed * _deltaTime;

    // Move the player using the arrow keys
    if (m_windowC->GetKeyStates()[VK_RIGHT]) pos.x += movement;
    if (m_windowC->GetKeyStates()[VK_LEFT]) pos.x -= movement;
    if (m_windowC->GetKeyStates()[VK_UP]) pos.y += movement;
    if (m_windowC->GetKeyStates()[VK_DOWN]) pos.y -= movement;

    m_player->SetPosition(XMFLOAT3(pos.x, pos.y, pos.z));

    // Update NPCs only when the player moves
    if (pos.x != previousPosition.x || pos.y != previousPosition.y)
    {
        UpdateNPCs(_deltaTime);
    }
}

void GameController::UpdateNPCs(float _deltaTime)
{
    for (auto npc : m_npcs)
    {
        npc->Update(m_player->GetPosition(), _deltaTime);
    }
}

void GameController::RenderScene(const wstring& _fpsS)
{
    FontController::GetInstance().Write(_fpsS, L"Arial_20", D2D1_RECT_F(20, 80, 800, 120), D2D1::ColorF::Yellow);
    m_shaderSolidColor->ResetCurrentObjectIndex();
    m_renderer->Render(m_units, Camera::Current()->GetViewProjection());
}

void GameController::SceneCleanup()
{
    // Delete all scene objects
    for (auto u : m_units)
    {
        delete u;
    }

    m_units.clear();
    m_npcs.clear();
    m_player = nullptr;


    // Delete shaders
    delete m_shaderSolidColor;
    m_shaderSolidColor = nullptr;

    // Delete the camera
    delete Camera::Current();
    Camera::SetCurrent(nullptr);
}







void GameController::Run()
{
    m_toolsC->ShowForm();

    SceneInitialize();

    //Main message loop
    bool running = true;
    double lastTime = GetTime();
    double lastFrameTime = lastTime;
    int fps = 0;
    wstring fpsS = L"0";

    while (running)
    {
        running = ProcessMessages();

        if (!running) break;

        double currentTime = GetTime();

        // Calculate the time elapsed since the previous frame
        float deltaTime = (float)(currentTime - lastFrameTime);
        lastFrameTime = currentTime;

        fps++;

        if (currentTime - lastTime >= 1)
        {
            fpsS = L"FPS: " + to_wstring(fps);
            fps = 0;
            lastTime = currentTime;
        }

        HandleInput(deltaTime);
        RenderScene(fpsS);
    }

    SceneCleanup();
}