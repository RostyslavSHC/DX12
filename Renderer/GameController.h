#pragma once

#include "StandardIncludes.h"

class Renderer;
class ToolsController;
class ShaderSolidColor;
class ShaderDistanceTint;
class Unit;
class NPC;

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
    double GetTime();


    void SceneInitialize();
    void InitializeBackground();
    void InitializeNPCs();
    bool ProcessMessages();
    void HandleInput(float _deltaTime);
    void UpdateScene(float _deltaTime);
    void UpdateNPCs(float _deltaTime);
    void RenderScene(const wstring& _fpsS);
    void SceneCleanup();

    // Members
    WindowController* m_windowC;
    Renderer* m_renderer;
    // Camera* m_camera;
    ToolsController* m_toolsC;

    // Scene Members
    vector<IRenderable*> m_units;
    ShaderSolidColor* m_shaderSolidColor;
    ShaderDistanceTint* m_shaderDistanceTint;
    Unit* m_player;
    Unit* m_background;
    // vector<Unit*> m_npcs;
    vector<NPC*> m_npcs;
    float m_spawnTime;
};