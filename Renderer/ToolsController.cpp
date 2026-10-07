#include "ToolsController.h"

// Default constructor
ToolsController::ToolsController()
{
    m_module = nullptr;
    m_showForm = nullptr;
    m_getYData = nullptr;
    m_getUData = nullptr;
    m_getVData = nullptr;
    m_getInvertData = nullptr;
}

// Load the tools library
void ToolsController::LoadToolsLibrary()
{
    m_module = LoadLibraryW(L"ToolsGUI.dll"); // Load the tools library
    M_ASSERT(m_module != nullptr, "Failed to load ToolsGUI.dll"); // ERROR HERE, had to fix it

    // Get the address of ShowToolsForm function
    FARPROC address = GetProcAddress(m_module, "ShowToolsForm");
    m_showForm = reinterpret_cast<ShowToolsForm>(address);

    // Get address of GetToolsYData function
    address = GetProcAddress(m_module, "GetToolsYData");
    m_getYData = reinterpret_cast<GetToolsYData>(address);

    // Get address of GetToolsUData function
    address = GetProcAddress(m_module, "GetToolsUData");
    m_getUData = reinterpret_cast<GetToolsUData>(address);

    // Get address of GetToolsVData function
    address = GetProcAddress(m_module, "GetToolsVData");
    m_getVData = reinterpret_cast<GetToolsVData>(address);

    // Get address of GetToolsInvertData function
    address = GetProcAddress(m_module, "GetToolsInvertData");
    m_getInvertData = reinterpret_cast<GetToolsInvertData>(address);
}