#include "GameController.h"
#include "Renderer.h"
#include "ToolsController.h"
#include "Unit.h"
#include "ShaderPhong.h"

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

	ShaderPhong shader = ShaderPhong();
	shader.Initialize();
	Unit unit = Unit();
	unit.Initialize(&shader);

	//Main message loop
	MSG msg = {};
	bool running = true;
	while (running)
	{
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

		auto pos = unit.GetPosition();
		auto rot = unit.GetRotation();
		auto scl = unit.GetScale();

		if (m_windowC->GetKeyStates()[VK_RIGHT]) pos.x += 0.01f;
		if (m_windowC->GetKeyStates()[VK_LEFT]) pos.x -= 0.01f;
		if (m_windowC->GetKeyStates()[VK_UP]) pos.y += 0.01f;
		if (m_windowC->GetKeyStates()[VK_DOWN]) pos.y -= 0.01f;

		if (m_windowC->GetKeyStates()['Q']) rot.y += 0.1f;
		if (m_windowC->GetKeyStates()['W']) rot.y -= 0.1f;
		if (m_windowC->GetKeyStates()['A']) scl.x -= 0.1f;
		if (m_windowC->GetKeyStates()['S']) scl.x += 0.1f;

		unit.SetPosition(XMFLOAT3(pos.x, pos.y, pos.z));
		unit.SetRotation(XMFLOAT3(rot.x, rot.y, rot.z));
		unit.SetScale(XMFLOAT3(scl.x, scl.y, scl.z));

		m_renderer->Render(&unit, m_camera->GetViewProjection());
	}
}
