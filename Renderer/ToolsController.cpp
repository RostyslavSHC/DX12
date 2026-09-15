#include "ToolsController.h"

// Default constructor
ToolsController::ToolsController()
{
    m_module = nullptr;
    m_showForm = nullptr;
    m_getRedData = nullptr;
    m_getGreenData = nullptr;
    m_getBlueData = nullptr;
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
}