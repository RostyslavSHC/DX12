#pragma once

#include "StandardIncludes.h"

class ToolsController : public Singleton<ToolsController>
{
public:
    // Constructors/Destructors
    ToolsController();
    virtual ~ToolsController() {}

    // Typedefs
    typedef void(__stdcall* ShowToolsForm)();
    typedef bool(__stdcall* GetToolsRedData)();
    typedef bool(__stdcall* GetToolsGreenData)();
    typedef bool(__stdcall* GetToolsBlueData)();
    typedef bool(__stdcall* GetToolsRenderBackgroundData)();
    typedef bool(__stdcall* GetToolsRenderSpotlightsData)();

    // Accessors
    void ShowForm() const { return m_showForm(); } // Show the form
    int GetRedData() const { return m_getRedData(); } // Get the red data
    int GetGreenData() const { return m_getGreenData(); } // Get the green data
    int GetBlueData() const { return m_getBlueData(); } // Get the blue data
    bool GetRenderBackgroundData() const;
    bool GetRenderSpotlightsData() const;

    // Methods
    void LoadToolsLibrary(); // Load the tools library

private:
    // Members
    HMODULE m_module;
    ShowToolsForm m_showForm;
    GetToolsRedData m_getRedData;
    GetToolsGreenData m_getGreenData;
    GetToolsBlueData m_getBlueData;
    GetToolsRenderBackgroundData m_getRenderBackgroundData;
    GetToolsRenderSpotlightsData m_getRenderSpotlightsData;
};