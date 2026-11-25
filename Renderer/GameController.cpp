#include "GameController.h"
#include "Renderer.h"
#include "ToolsController.h"
#include "Unit.h"
#include "ShaderPhong.h"
#include "ShaderColor.h"
#include "ShaderSkybox.h"
#include "ShaderSolidColor.h"
#include "NPC.h"
#include "ShaderDistanceTint.h"

GameController::GameController()
{
    m_windowC = nullptr;
    m_renderer = nullptr;
    // m_camera = nullptr;
    m_toolsC = nullptr;

    m_shaderSolidColor = nullptr;
    m_player = nullptr;
    m_spawnTime = 0.0f;
    m_shaderDistanceTint = nullptr;
    m_background = nullptr;
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
    // m_shaderSolidColor->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
    // m_shaderSolidColor->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f });
    m_shaderSolidColor->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });

    m_shaderDistanceTint = new ShaderDistanceTint();
    m_shaderDistanceTint->Initialize();
    m_shaderDistanceTint->InitializeTexture("..\\Assets\\Textures\\Assignment2Wood.dds");

    // Configure the camera
    Camera::Current()->SetView(
        { 0.0f, 0.0f, -40.0f },
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f });

    // Initialize the background
    InitializeBackground();

    // Initialize the player
    m_player = new Unit();
    m_player->Initialize("..\\Assets\\Models\\Cube.obj", m_shaderSolidColor);
    m_player->SetPosition({ 0.0f, 0.0f, 0.0f });
    // m_player->SetScale({ 2.0f, 2.0f, 2.0f });
    m_player->SetScale({ 2.5f, 2.5f, 2.5f });

    m_units.push_back(m_player);

    InitializeNPCs(); // Initialize NPCs
}





void GameController::InitializeBackground()
{
    // Calculate the visible background dimensions
    float depth = 42.0f;

    float height = 2.0f * depth * tanf(XMConvertToRadians(45.0f) / 2.0f);

    float aspectRatio = (float)m_windowC->GetWindowWidth() / (float)m_windowC->GetWindowHeight();

    float width = height * aspectRatio;

    // Initialize the background
    m_background = new Unit();
    m_background->Initialize("..\\Assets\\Models\\Background.obj", m_shaderDistanceTint);

    m_background->SetPosition({ 0.0f, 0.0f, 2.0f });
    // m_background->SetScale({ width, height, 1.0f });
    // float overscale = 1.2f;
    // m_background->SetScale({ width * overscale, height * overscale, 1.0f });

    // Scale the background to match the reference
    // m_background->SetScale({ width * 1.6f, height * 1.45f, 1.0f });
    m_background->SetScale({ width * 1.6f, height * 1.25f, 1.0f });

    m_units.push_back(m_background);
}












void GameController::InitializeNPCs()
{
    const int npcCount = 8;
    const float minRadius = 2.0f;
    const float maxRadius = 4.0f;

    for (int i = 0; i < npcCount; i++)
    {
        // Calculate a random position around the player
        float angle = RndF(0.0f, 1.0f) * XM_2PI;

        float radius = minRadius +
            RndF(0.0f, 1.0f) * (maxRadius - minRadius);

        float x = cosf(angle) * radius;
        float y = sinf(angle) * radius;

        // Initialize the NPC
        NPC* npc = new NPC();
        // npc->Initialize("..\\Assets\\Models\\Cube.obj", m_shaderSolidColor);
        npc->Initialize("..\\Assets\\Models\\Cube.obj", m_shaderDistanceTint);

        npc->SetPosition({ x, y, 0.0f });
        npc->SetScale({ 2.5f, 2.5f, 2.5f });

        // Face away from the player
        npc->SetRotation({
            0.0f,
            0.0f,
            angle - XM_PIDIV2
            });

        m_npcs.push_back(npc);
        m_units.push_back(npc);
    }

    // Allow NPCs to spread out at the beginning
    m_spawnTime = 2.0f;
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

    float speed = 10.0f;
    float movement = speed * _deltaTime;

    // Move the player using the arrow keys
    if (m_windowC->GetKeyStates()[VK_RIGHT]) pos.x += movement;
    if (m_windowC->GetKeyStates()[VK_LEFT]) pos.x -= movement;
    if (m_windowC->GetKeyStates()[VK_UP]) pos.y += movement;
    if (m_windowC->GetKeyStates()[VK_DOWN]) pos.y -= movement;

    m_player->SetPosition(XMFLOAT3(pos.x, pos.y, pos.z));
}


void GameController::UpdateScene(float _deltaTime)
{
    XMFLOAT3 previousPosition = m_player->GetPosition();

    HandleInput(_deltaTime);

    XMFLOAT3 currentPosition = m_player->GetPosition();

    // Check whether the player has moved
    bool playerMoved =
        currentPosition.x != previousPosition.x ||
        currentPosition.y != previousPosition.y;

    // Update NPCs during the initial spreading phase
    if (m_spawnTime > 0.0f)
    {
        UpdateNPCs(_deltaTime);
        m_spawnTime -= _deltaTime;
    }
    else if (playerMoved)
    {
        // Update NPCs only when the player moves
        UpdateNPCs(_deltaTime);
    }
}







void GameController::UpdateNPCs(float _deltaTime)
{
    XMFLOAT3 playerPosition = m_player->GetPosition();

    for (auto npc : m_npcs)
    {
        // Prevent tagging during the initial spreading phase
        if (m_spawnTime <= 0.0f)
        {
            npc->CheckTag(playerPosition);
        }

        npc->Update(playerPosition, _deltaTime);
    }
}

void GameController::RenderScene(const wstring& _fpsS)
{
    FontController::GetInstance().Write(_fpsS, L"Arial_20", D2D1_RECT_F(20, 80, 800, 120), D2D1::ColorF::Yellow);

    // Update the player position used for distance tinting
    m_shaderDistanceTint->SetPlayerPosition(m_player->GetPosition());

    m_shaderSolidColor->ResetCurrentObjectIndex();
    m_shaderDistanceTint->ResetCurrentObjectIndex();

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
    m_background = nullptr;


    // Delete shaders
    delete m_shaderSolidColor;
    m_shaderSolidColor = nullptr;
    delete m_shaderDistanceTint;
    m_shaderDistanceTint = nullptr;

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

        UpdateScene(deltaTime);
        RenderScene(fpsS);
    }

    SceneCleanup();
}