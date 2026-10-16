#include "GameController.h"
#include "Renderer.h"
#include "ToolsController.h"
#include "Unit.h"
#include "ShaderPhong.h"
#include "ShaderColor.h"
#include "ShaderPositionColor.h"

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


	vector<IRenderable*> units;
	ShaderColor shaderColor = ShaderColor();
	shaderColor.Initialize();
	Unit light = Unit();
	light.Initialize(&shaderColor);
	light.SetPosition(XMFLOAT3(0.5f, 0.5f, -2.0f));
	light.SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	units.push_back(&light);

	ShaderPhong shaderPhong = ShaderPhong();
	shaderPhong.Initialize();
	Unit unit = Unit();
	unit.Initialize(&shaderPhong);
	units.push_back(&unit);

	// Create the shader for the position-based color effect
	ShaderPositionColor shaderPositionColor = ShaderPositionColor();
	shaderPositionColor.Initialize();
	// Create the brick-textured cube
	Unit brickUnit = Unit();
	// brickUnit.Initialize(&shaderPositionColor, L"..\\Assets\\Textures\\BrickWall.dds");
	brickUnit.Initialize(&shaderPositionColor,
		L"..\\Assets\\Textures\\BrickWall.dds",
		L"..\\Assets\\Textures\\Flowers.dds");

	// Store the brick cube for Color By Position mode
	vector<IRenderable*> brickUnits;
	brickUnits.push_back(&brickUnit);



	//ShaderPhong shader = ShaderPhong();vector<IRenderable*> brickUnits;

	//shader.Initialize();
	//Unit unit = Unit();
	//unit.Initialize(&shader);

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






		/*
		auto pos = light.GetPosition(); // Get the current position of the red light cube

		// Move the light on the X axis
		if (m_windowC->GetKeyStates()[VK_RIGHT]) pos.x += 0.01f;
		if (m_windowC->GetKeyStates()[VK_LEFT]) pos.x -= 0.01f;

		// Move the light on the Y axis
		if (m_windowC->GetKeyStates()[VK_UP]) pos.y += 0.01f;
		if (m_windowC->GetKeyStates()[VK_DOWN]) pos.y -= 0.01f;

		// Move the light on the Z axis
		if (m_windowC->GetKeyStates()['Z']) pos.z += 0.01f;
		if (m_windowC->GetKeyStates()['X']) pos.z -= 0.01f;
		




		light.SetPosition(pos); // Update the red light cube's position

		*/


		if (m_toolsC->GetColorByPositionData())
		{
			// Move the brick cube in Color By Position mode
			auto pos = brickUnit.GetPosition();

			if (m_windowC->GetKeyStates()[VK_RIGHT]) pos.x += 0.01f;
			if (m_windowC->GetKeyStates()[VK_LEFT]) pos.x -= 0.01f;
			if (m_windowC->GetKeyStates()[VK_UP]) pos.y += 0.01f;
			if (m_windowC->GetKeyStates()[VK_DOWN]) pos.y -= 0.01f;
			if (m_windowC->GetKeyStates()['Z']) pos.z += 0.01f;
			if (m_windowC->GetKeyStates()['X']) pos.z -= 0.01f;

			brickUnit.SetPosition(pos);
		}
		else
		{
			// Move the red light cube in Move Light mode
			auto pos = light.GetPosition();

			// Move the light on the X axis
			if (m_windowC->GetKeyStates()[VK_RIGHT]) pos.x += 0.01f;
			if (m_windowC->GetKeyStates()[VK_LEFT]) pos.x -= 0.01f;

			// Move the light on the Y axis
			if (m_windowC->GetKeyStates()[VK_UP]) pos.y += 0.01f;
			if (m_windowC->GetKeyStates()[VK_DOWN]) pos.y -= 0.01f;

			// Move the light on the Z axis
			if (m_windowC->GetKeyStates()['Z']) pos.z += 0.01f;
			if (m_windowC->GetKeyStates()['X']) pos.z -= 0.01f;

			light.SetPosition(pos); // Update the red light cube's position
		}



















		// Reset the light to its original position when the button is clicked
		if (m_toolsC->GetResetLightData())
		{
			light.SetPosition(XMFLOAT3(0.5f, 0.5f, -2.0f));
		}

		shaderPhong.SetLightPosition(light.GetPosition()); // Make the light follow the red cube
		shaderPhong.SetSpecularStrength(m_toolsC->GetSpecularStrengthData()); // Update specular strength from the TrackBar

		// Update the specular color using the RGB TrackBars
		shaderPhong.SetSpecularColor(XMFLOAT4(
			m_toolsC->GetSpecularRedData(),
			m_toolsC->GetSpecularGreenData(),
			m_toolsC->GetSpecularBlueData(),
			1.0f
		));








		// Check whether Color By Position mode is selected
		if (m_toolsC->GetColorByPositionData())
		{
			// Render only the brick cube
			// vector<IRenderable*> brickUnits;
			// brickUnits.push_back(&brickUnit);

			m_renderer->Render(brickUnits, m_camera->GetViewProjection());
		}
		else
		{
			// Render the red light cube and wooden crate
			m_renderer->Render(units, m_camera->GetViewProjection());
		}






		// m_renderer->Render(units, m_camera->GetViewProjection());
	}
}
