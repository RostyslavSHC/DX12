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

// Get Y trackbar value
extern "C" __declspec(dllexport)
int __stdcall GetToolsYData()
{
    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];
    return form->trackBarY->Value;
}

// Get U trackbar value
extern "C" __declspec(dllexport)
int __stdcall GetToolsUData()
{
    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];
    return form->trackBarU->Value;
}

// Get V trackbar value
extern "C" __declspec(dllexport)
int __stdcall GetToolsVData()
{
    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];
    return form->trackBarV->Value;
}

// Get if Invert is checked
extern "C" __declspec(dllexport)
bool __stdcall GetToolsInvertData()
{
    ToolsForm^ form = (ToolsForm^)Application::OpenForms[0];
    return form->checkBoxInvert->Checked;
}