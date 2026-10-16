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





// Get the current specular strength from the TrackBar
extern "C" __declspec(dllexport)
float __stdcall GetToolsSpecularStrengthData()
{
    if (Application::OpenForms->Count == 0)
        return 0.0f;

    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];

    return static_cast<float>(form->trackBarSpecularStrength->Value) / 5.0f;
}






extern "C" __declspec(dllexport)
float __stdcall GetToolsSpecularRedData()
{
    if (Application::OpenForms->Count == 0)
        return 0.0f;

    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];

    return static_cast<float>(form->trackBarSpecularRed->Value) / 200.0f;
}

extern "C" __declspec(dllexport)
float __stdcall GetToolsSpecularGreenData()
{
    if (Application::OpenForms->Count == 0)
        return 0.0f;

    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];

    return static_cast<float>(form->trackBarSpecularGreen->Value) / 200.0f;
}

extern "C" __declspec(dllexport)
float __stdcall GetToolsSpecularBlueData()
{
    if (Application::OpenForms->Count == 0)
        return 0.0f;

    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];

    return static_cast<float>(form->trackBarSpecularBlue->Value) / 200.0f;
}







// Check whether Color By Position mode is selected
extern "C" __declspec(dllexport)
bool __stdcall GetToolsColorByPositionData()
{
    if (Application::OpenForms->Count == 0)
        return false;

    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];

    return form->radioButtonColorByPosition->Checked;
}







// Check whether the cube position should be reset
extern "C" __declspec(dllexport)
bool __stdcall GetToolsResetCubeData()
{
    if (Application::OpenForms->Count == 0)
        return false;

    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];

    bool reset = form->resetCubeRequested; // Check whether reset was requested
    form->resetCubeRequested = false; // Clear the request after reading it

    return reset;
}