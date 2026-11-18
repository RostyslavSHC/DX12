#pragma once

#include "StandardIncludes.h"

class Renderer;
class ToolsController;

class GameController : public Singleton<GameController>
{
public:
    // Constructors/Destructors
    GameController();
    virtual ~GameController();

    // Methods
    void Initialize(HINSTANCE _hInstance, int _nShowCmd);
    void Run();

private:
    // Methods
	float RndF(float _min, float _max);

	// Members
    WindowController* m_windowC;
    Renderer* m_renderer;
    // Camera* m_camera;
    ToolsController* m_toolsC;
};