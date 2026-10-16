#include "ToolsController.h"

// Default constructor
ToolsController::ToolsController()
{
    m_module = nullptr;
    m_showForm = nullptr;
    m_getRedData = nullptr;
    m_getGreenData = nullptr;
    m_getBlueData = nullptr;
    m_getResetLightData = nullptr;
    m_getSpecularStrengthData = nullptr;
    m_getSpecularRedData = nullptr;
    m_getSpecularGreenData = nullptr;
    m_getSpecularBlueData = nullptr;
}

// Load the tools library
void ToolsController::LoadToolsLibrary()
{
    m_module = LoadLibraryW(L"ToolsGUI.dll"); // Load the tools library
    M_ASSERT(m_module != nullptr, "Failed to load ToolsGUI.dll"); // ERROR HERE, had to fix it

    // Get the address of the ShowToolsForm function
    FARPROC address = GetProcAddress(m_module, "ShowToolsForm"); 
    m_showForm = reinterpret_cast<ShowToolsForm>(address);

    // Get the address of the GetToolsRedData function
    address = GetProcAddress(m_module, "GetToolsRedData");
    m_getRedData = reinterpret_cast<GetToolsRedData>(address);

    // Get the address of the GetToolsGreenData function
    address = GetProcAddress(m_module, "GetToolsGreenData");
    m_getGreenData = reinterpret_cast<GetToolsGreenData>(address);

    // Get the address of the GetToolsBlueData function
    address = GetProcAddress(m_module, "GetToolsBlueData");
    m_getBlueData = reinterpret_cast<GetToolsBlueData>(address);

    // Get the address of the GetToolsResetLightData function
    address = GetProcAddress(m_module, "GetToolsResetLightData");
    m_getResetLightData = reinterpret_cast<GetToolsResetLightData>(address);
    M_ASSERT(m_getResetLightData != nullptr, "Failed to load GetToolsResetLightData");

    // Get the address of the GetToolsSpecularStrengthData function
    address = GetProcAddress(m_module, "GetToolsSpecularStrengthData");
    m_getSpecularStrengthData = reinterpret_cast<GetToolsSpecularStrengthData>(address);
    M_ASSERT(m_getSpecularStrengthData != nullptr, "Failed to load GetToolsSpecularStrengthData");






    // Get the address of the GetToolsSpecularRedData function
    address = GetProcAddress(m_module, "GetToolsSpecularRedData");
    m_getSpecularRedData = reinterpret_cast<GetToolsSpecularRedData>(address);
    M_ASSERT(m_getSpecularRedData != nullptr, "Failed to load GetToolsSpecularRedData");

    // Get the address of the GetToolsSpecularGreenData function
    address = GetProcAddress(m_module, "GetToolsSpecularGreenData");
    m_getSpecularGreenData = reinterpret_cast<GetToolsSpecularGreenData>(address);
    M_ASSERT(m_getSpecularGreenData != nullptr, "Failed to load GetToolsSpecularGreenData");

    // Get the address of the GetToolsSpecularBlueData function
    address = GetProcAddress(m_module, "GetToolsSpecularBlueData");
    m_getSpecularBlueData = reinterpret_cast<GetToolsSpecularBlueData>(address);
    M_ASSERT(m_getSpecularBlueData != nullptr, "Failed to load GetToolsSpecularBlueData");

}

bool ToolsController::GetResetLightData() const
{
    return m_getResetLightData(); // Read the reset request from ToolsGUI
}

float ToolsController::GetSpecularStrengthData() const
{
    return m_getSpecularStrengthData(); // Read the specular strength from ToolsGUI
}






float ToolsController::GetSpecularRedData() const
{
    return m_getSpecularRedData();
}

float ToolsController::GetSpecularGreenData() const
{
    return m_getSpecularGreenData();
}

float ToolsController::GetSpecularBlueData() const
{
    return m_getSpecularBlueData();
}
