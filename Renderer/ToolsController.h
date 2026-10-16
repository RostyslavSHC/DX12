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
    typedef bool(__stdcall* GetToolsResetLightData)();
    typedef float(__stdcall* GetToolsSpecularStrengthData)();
    typedef float(__stdcall* GetToolsSpecularRedData)();
    typedef float(__stdcall* GetToolsSpecularGreenData)();
    typedef float(__stdcall* GetToolsSpecularBlueData)();
    typedef bool(__stdcall* GetToolsColorByPositionData)();

    // Accessors
    void ShowForm() const { return m_showForm(); } // Show the form
    int GetRedData() const { return m_getRedData(); } // Get the red data
    int GetGreenData() const { return m_getGreenData(); } // Get the green data
    int GetBlueData() const { return m_getBlueData(); } // Get the blue data
    bool GetResetLightData() const; // Check whether the reset button was clicked
    float GetSpecularStrengthData() const; // Get the specular strength from ToolsGUI
    float GetSpecularRedData() const; // Get the red specular component
    float GetSpecularGreenData() const; // Get the green specular component
    float GetSpecularBlueData() const; // Get the blue specular component
    bool GetColorByPositionData() const; // Check whether Color By Position mode is selected

    // Methods
    void LoadToolsLibrary(); // Load the tools library

private:
    // Members
    HMODULE m_module;
    ShowToolsForm m_showForm;
    GetToolsRedData m_getRedData;
    GetToolsGreenData m_getGreenData;
    GetToolsBlueData m_getBlueData;
    GetToolsResetLightData m_getResetLightData; // DLL function for light reset
    GetToolsSpecularStrengthData m_getSpecularStrengthData; // DLL function for specular strength
    GetToolsSpecularRedData m_getSpecularRedData;
    GetToolsSpecularGreenData m_getSpecularGreenData;
    GetToolsSpecularBlueData m_getSpecularBlueData;
    GetToolsColorByPositionData m_getColorByPositionData; // DLL function for Color By Position mode
};