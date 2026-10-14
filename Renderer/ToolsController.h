
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
    typedef void(__stdcall* ShowSpecularForm)();
    typedef int(__stdcall* GetSpecularPower)();

    // Accessors
    void ShowForm() const { return m_showForm(); } // Show the form
    int GetRedData() const { return m_getRedData(); } // Get the red data
    int GetGreenData() const { return m_getGreenData(); } // Get the green data
    int GetBlueData() const { return m_getBlueData(); } // Get the blue data
    void ShowSpecular() const { return m_showSpecularForm(); } // Show the specular form
    int GetSpecularData() const { return m_getSpecularPower(); } // Get the specular power

    // Methods
    void LoadToolsLibrary(); // Load the tools library

private:
    // Members
    HMODULE m_module;
    ShowToolsForm m_showForm;
    GetToolsRedData m_getRedData;
    GetToolsGreenData m_getGreenData;
    GetToolsBlueData m_getBlueData;
    ShowSpecularForm m_showSpecularForm;
    GetSpecularPower m_getSpecularPower;
};
