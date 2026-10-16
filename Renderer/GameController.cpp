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
	
	// Tools Form and Scene Setup
	m_toolsC->ShowForm();


	//  -------------------- MODE 1 - MOVE LIGHT: Scene Setup --------------------
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



	// -------------------- MODE 2 - COLOR BY POSITION: Scene Setup --------------------
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

	// -------------------- MODE 3 - MOVE LIGHT TO CUBE: Scene Setup --------------------
	/*
	// Create the flower-textured target cube for Move Light To Cube mode
	Unit targetCube = Unit();
	targetCube.Initialize(&shaderPositionColor,
		L"..\\Assets\\Textures\\BrickWall.dds",
		L"..\\Assets\\Textures\\Flowers.dds");
	targetCube.SetPosition(XMFLOAT3(-1.5f, 0.0f, 0.0f));
	*/



	// Create a separate shader for the flower-textured target cube
	ShaderPositionColor shaderTarget = ShaderPositionColor();
	shaderTarget.Initialize();

	// Create the flower-textured target cube for Move Light To Cube mode
	Unit targetCube = Unit();
	targetCube.Initialize(&shaderTarget,
		L"..\\Assets\\Textures\\BrickWall.dds",
		L"..\\Assets\\Textures\\Flowers.dds");

	// Position the flower cube slightly above and left of the wooden crate
	targetCube.SetPosition(XMFLOAT3(-0.6f, 0.4f, 2.0f));
	targetCube.SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));

	// Wooden crate in the middle
	//unit.SetScale(XMFLOAT3(0.6f, 0.6f, 0.6f));


	// Create a separate shader for the chasing red light
	ShaderColor shaderChasingLight = ShaderColor();
	shaderChasingLight.Initialize();

	// Create the red light cube that automatically follows the target
	Unit chasingLight = Unit();
	chasingLight.Initialize(&shaderChasingLight);


	// chasingLight.SetPosition(XMFLOAT3(1.5f, -1.0f, -1.5f));
	// Create the red light's starting position near the bottom-right camera edge
	float spawnDistance = 0.8f; // Distance in front of the camera
	float halfHeight = spawnDistance * tanf(XMConvertToRadians(45.0f) / 2.0f);
	float halfWidth = halfHeight *
		((float)m_windowC->GetWindowWidth() / (float)m_windowC->GetWindowHeight());

	// Camera looks from (0, 1, -3) toward the origin
	XMVECTOR cameraPosition = XMVectorSet(0.0f, 1.0f, -3.0f, 0.0f);
	XMVECTOR forward = XMVector3Normalize(
		XMVectorSet(0.0f, -1.0f, 3.0f, 0.0f));

	XMVECTOR right = XMVector3Normalize(
		XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), forward));

	XMVECTOR up = XMVector3Normalize(
		XMVector3Cross(forward, right));

	// Place the light slightly outside the bottom-right edge of the camera view
	XMVECTOR spawnVector = XMVectorAdd(
		cameraPosition,
		XMVectorAdd(
			XMVectorScale(forward, spawnDistance),
			XMVectorAdd(
				XMVectorScale(right, halfWidth * 1.1f),
				XMVectorScale(up, -halfHeight * 1.1f))));

	XMFLOAT3 chasingLightStart;
	XMStoreFloat3(&chasingLightStart, spawnVector);

	chasingLight.SetPosition(chasingLightStart);
	chasingLight.SetScale(XMFLOAT3(0.1f, 0.1f, 0.1f));


	// Create a separate shader for the wooden crate in Mode 3
	ShaderPhong shaderMode3Crate = ShaderPhong();
	shaderMode3Crate.Initialize();

	// Create the wooden crate for Mode 3
	Unit mode3Crate = Unit();
	mode3Crate.Initialize(&shaderMode3Crate);

	// Keep the wooden crate centered in Mode 3
	mode3Crate.SetPosition(XMFLOAT3(0.0f, 0.0f, 0.6f));
	mode3Crate.SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));


	// Store all three cubes for Move Light To Cube mode
	vector<IRenderable*> lightToCubeUnits;
	lightToCubeUnits.push_back(&targetCube);
	lightToCubeUnits.push_back(&chasingLight);
	lightToCubeUnits.push_back(&mode3Crate);


	//ShaderPhong shader = ShaderPhong();vector<IRenderable*> brickUnits;

	//shader.Initialize();
	//Unit unit = Unit();
	//unit.Initialize(&shader);








	// -------------------- SHARED - Main Message Loop --------------------
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
			// -------------------- MODE 2 - COLOR BY POSITION: Keyboard Movement --------------------
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
		else if (m_toolsC->GetMoveLightToCubeData())
		{
			// -------------------- MODE 3 - MOVE LIGHT TO CUBE: Target Movement --------------------
			// Move the flower-textured target cube in Move Light To Cube mode
			auto pos = targetCube.GetPosition();

			if (m_windowC->GetKeyStates()[VK_RIGHT]) pos.x += 0.01f;
			if (m_windowC->GetKeyStates()[VK_LEFT]) pos.x -= 0.01f;
			if (m_windowC->GetKeyStates()[VK_UP]) pos.y += 0.01f;
			if (m_windowC->GetKeyStates()[VK_DOWN]) pos.y -= 0.01f;
			if (m_windowC->GetKeyStates()['Z']) pos.z += 0.01f;
			if (m_windowC->GetKeyStates()['X']) pos.z -= 0.01f;

			targetCube.SetPosition(pos);
		}
		else
		{
			// -------------------- MODE 1 - MOVE LIGHT: Keyboard Movement --------------------
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


		// -------------------- MODE 1 - MOVE LIGHT: Reset Light Position --------------------
			// Reset the light to its original position when the button is clicked
		if (m_toolsC->GetResetLightData())
		{
			light.SetPosition(XMFLOAT3(0.5f, 0.5f, -2.0f));
		}

			/*
			// Check whether the red light touches the cube in Move Light To Cube mode
			if (m_toolsC->GetMoveLightToCubeData())
			{
				XMFLOAT3 lightPos = light.GetPosition();
				XMFLOAT3 cubePos = unit.GetPosition();

				// The cubes overlap when their centers are close enough on all three axes
				bool collisionX = abs(lightPos.x - cubePos.x) <= 0.55f;
				bool collisionY = abs(lightPos.y - cubePos.y) <= 0.55f;
				bool collisionZ = abs(lightPos.z - cubePos.z) <= 0.55f;

				if (collisionX && collisionY && collisionZ)
				{
					//MessageBoxA(NULL, "Collision detected!", "Collision Test", MB_OK); // Test collision
					light.SetPosition(XMFLOAT3(0.5f, 0.5f, -2.0f)); // Reset the light after collision
				}
			}
			*/



			// -------------------- MODE 3 - MOVE LIGHT TO CUBE: Automatic Movement and Center Reset --------------------
		// Automatically move the red light toward the flower cube
		if (m_toolsC->GetMoveLightToCubeData())
		{
			XMFLOAT3 lightPos = chasingLight.GetPosition();
			XMFLOAT3 cubePos = targetCube.GetPosition();

			// Calculate the direction from the red light to the cube's center
			XMVECTOR lightVector = XMLoadFloat3(&lightPos);
			XMVECTOR cubeVector = XMLoadFloat3(&cubePos);
			XMVECTOR direction = XMVectorSubtract(cubeVector, lightVector);

			// Calculate the distance between their centers
			float distance = XMVectorGetX(XMVector3Length(direction));

			// Reset the red light when it reaches the cube's center
			if (distance <= 0.12f)
			{
				chasingLight.SetPosition(chasingLightStart); // Reset to the same camera-relative starting position
			}
			else
			{
				// Move the red light a small step toward the cube
				XMVECTOR normalizedDirection = XMVector3Normalize(direction);
				XMVECTOR movement = XMVectorScale(normalizedDirection, 0.008f);
				XMVECTOR newPosition = XMVectorAdd(lightVector, movement);

				XMFLOAT3 newLightPos;
				XMStoreFloat3(&newLightPos, newPosition);

				chasingLight.SetPosition(newLightPos);
			}
		}


		// -------------------- MODE 2 - COLOR BY POSITION: Reset Cube Position --------------------
			// Reset the brick cube to its original position when the button is clicked
		if (m_toolsC->GetResetCubeData())
		{
			brickUnit.SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
		}


		// shaderPhong.SetLightPosition(light.GetPosition()); // Make the light follow the red cube


		// --------------------SHARED - Light Position for Mode 1 and Mode 3 --------------------
		// Make the wooden crate receive light from the correct light cube
		if (m_toolsC->GetMoveLightToCubeData())
		{
			shaderMode3Crate.SetLightPosition(chasingLight.GetPosition());
		}
		else
		{
			shaderPhong.SetLightPosition(light.GetPosition()); // Make the light follow the red cube
		}


		// -------------------- SHARED - Specular Strength  and RGB Settings --------------------
		shaderPhong.SetSpecularStrength(m_toolsC->GetSpecularStrengthData()); // Update specular strength from the TrackBar

		// Update the specular color using the RGB TrackBars
		shaderPhong.SetSpecularColor(XMFLOAT4(
			m_toolsC->GetSpecularRedData(),
			m_toolsC->GetSpecularGreenData(),
			m_toolsC->GetSpecularBlueData(),
			1.0f
		));



		// Update the specular settings for the Mode 3 wooden crate
		shaderMode3Crate.SetSpecularStrength(m_toolsC->GetSpecularStrengthData());

		shaderMode3Crate.SetSpecularColor(XMFLOAT4(
			m_toolsC->GetSpecularRedData(),
			m_toolsC->GetSpecularGreenData(),
			m_toolsC->GetSpecularBlueData(),
			1.0f
		));


		// -------------------- RENDER SCENE - Draw Only the Selected Mode --------------------
		// Check whether Color By Position mode is selected
		if (m_toolsC->GetColorByPositionData())
		{
			// Render only the brick cube
			// vector<IRenderable*> brickUnits;
			// brickUnits.push_back(&brickUnit);

			m_renderer->Render(brickUnits, m_camera->GetViewProjection());
		}
		else if (m_toolsC->GetMoveLightToCubeData())
		{
			// Render the red light and target cube in Move Light To Cube mode
			m_renderer->Render(lightToCubeUnits, m_camera->GetViewProjection());
		}
		else
		{
			// Render the red light cube and wooden crate
			m_renderer->Render(units, m_camera->GetViewProjection());
		}


		// m_renderer->Render(units, m_camera->GetViewProjection());
	}
}
