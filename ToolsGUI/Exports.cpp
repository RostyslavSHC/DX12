#include "ToolsForm.h"

using namespace ToolsGUI; // Namespace

//Extern means it can be used outside of the file
extern "C" __declspec(dllexport)
void __stdcall ShowToolsForm()
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    ToolsForm^ form = gcnew ToolsForm();
    form->Show();
}

extern "C" __declspec(dllexport)
bool __stdcall GetToolsRedData()
{
    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];
    return form->checkBoxRed->Checked;
}

extern "C" __declspec(dllexport)
bool __stdcall GetToolsGreenData()
{
    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];
    return form->checkBoxGreen->Checked;
}

extern "C" __declspec(dllexport)
bool __stdcall GetToolsBlueData()
{
    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];
    return form->checkBoxBlue->Checked;
}








extern "C" __declspec(dllexport)
bool __stdcall GetToolsResetLightData()
{
    if (Application::OpenForms->Count == 0)
        return false;

    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];

    bool reset = form->resetLightRequested; // Check whether reset was requested
    form->resetLightRequested = false; // Clear the request after reading it

    return reset;
}