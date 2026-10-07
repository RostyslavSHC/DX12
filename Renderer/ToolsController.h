#pragma once

#include "StandardIncludes.h"

class ToolsController : public Singleton<ToolsController>
{
public:
    // Constructors/Destructors
    ToolsController();
    virtual ~ToolsController() {}

    // Typedefs
    typedef void(__stdcall* ShowToolsForm)(); // Function pointer for showing the ToolsForm
    typedef int(__stdcall* GetToolsYData)(); // Function pointer for getting the Y trackbar data
    typedef int(__stdcall* GetToolsUData)(); // Function pointer for getting the U trackbar data
    typedef int(__stdcall* GetToolsVData)(); // Function pointer for getting the V trackbar data
    typedef bool(__stdcall* GetToolsInvertData)(); // Function pointer for getting the Invert Colors checkbox value

    // Accessors
    void ShowForm() const { return m_showForm(); } // Show the form
    int GetYData() const { return m_getYData(); } // Get the Y data
    int GetUData() const { return m_getUData(); } // Get the U data
    int GetVData() const { return m_getVData(); } // Get the V data
    bool GetInvertData() const { return m_getInvertData(); } // Get the invert data

    // Methods
    void LoadToolsLibrary(); // Load the tools library

private:
    // Members
    HMODULE m_module;
    ShowToolsForm m_showForm;
    GetToolsYData m_getYData;
    GetToolsUData m_getUData;
    GetToolsVData m_getVData;
    GetToolsInvertData m_getInvertData;
};