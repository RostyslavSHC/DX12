#include "GameController.h"
#include "Renderer.h"
#include "ToolsController.h"
#include "Unit.h"

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
	shader.Initialize(L"shaders.hlsl");
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




		ConstantBuffer* cbData = &shader.GetConstantData()[m_renderer->GetFrameIndex()]; // Get the current constant buffer data

		// Get the YUV values from the ToolsForm. I use 100.0f to normalize the values because the form returns values between 0 and 200, while shader needs 1.0. So 100% becomes 1.0f
		cbData->yValue = m_toolsC->GetYData() / 100.0f;
		cbData->uValue = m_toolsC->GetUData() / 100.0f;
		cbData->vValue = m_toolsC->GetVData() / 100.0f;

		// Get if Invert Colors is checked
		cbData->invertColors = m_toolsC->GetInvertData() ? 1.0f : 0.0f;





		auto pos = unit.GetPosition();
		auto rot = unit.GetRotation();
		auto scl = unit.GetScale();

		if (m_windowC->GetKeyStates()[VK_RIGHT]) pos.x += 0.01f;
		if (m_windowC->GetKeyStates()[VK_LEFT]) pos.x -= 0.01f;
		if (m_windowC->GetKeyStates()[VK_UP]) pos.y += 0.01f;
		if (m_windowC->GetKeyStates()[VK_DOWN]) pos.y -= 0.01f;

		if (m_windowC->GetKeyStates()['Q']) rot.y += 0.1f;
		if (m_windowC->GetKeyStates()['E']) rot.y -= 0.1f;
		if (m_windowC->GetKeyStates()['A']) scl.x -= 0.1f;
		if (m_windowC->GetKeyStates()['S']) scl.x += 0.1f;

		unit.SetPosition(XMFLOAT3(pos.x, pos.y, pos.z));
		unit.SetRotation(XMFLOAT3(rot.x, rot.y, rot.z));
		unit.SetScale(XMFLOAT3(scl.x, scl.y, scl.z));

		m_renderer->Render(&unit, m_camera->GetViewProjection());
	}
}
