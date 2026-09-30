#include "GameController.h"

GameController::GameController()
{
    m_windowC = nullptr;
    m_renderer = nullptr;
	m_camera = nullptr;
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
	m_camera = new Camera(m_windowC->GetWindowWidth(), m_windowC->GetWindowHeight()); // Create the camera
    m_toolsC = &ToolsController::GetInstance();
    m_toolsC->LoadToolsLibrary();
}

void GameController::Run()
{
	m_toolsC->ShowForm();

	Shader shader = Shader();
	shader.Initialize(L"shaders.hlsl", m_renderer->GetDevice().Get());
	Mesh mesh = Mesh();
	mesh.Initialize(m_renderer->GetDevice().Get(), &shader);

	//Main message loop
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_renderer->GetBGColor()[0] = (float)m_toolsC->GetRedData();
			m_renderer->GetBGColor()[1] = (float)m_toolsC->GetGreenData();
			m_renderer->GetBGColor()[2] = (float)m_toolsC->GetBlueData();

			m_renderer->Render(&mesh, m_camera->GetViewProjection());
		}
	}
}
