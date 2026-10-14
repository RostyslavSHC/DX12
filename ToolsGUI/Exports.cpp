
#include "ToolsForm.h"
#include "SpecularForm.h"

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
    for each (Form ^ form in Application::OpenForms)
    {
        ToolsForm^ toolsForm = dynamic_cast<ToolsForm^>(form);

        if (toolsForm != nullptr)
        {
            return toolsForm->checkBoxRed->Checked;
        }
    }

    return false;
}

extern "C" __declspec(dllexport)
bool __stdcall GetToolsGreenData()
{
    for each (Form ^ form in Application::OpenForms)
    {
        ToolsForm^ toolsForm = dynamic_cast<ToolsForm^>(form);

        if (toolsForm != nullptr)
        {
            return toolsForm->checkBoxGreen->Checked;
        }
    }

    return false;
}

extern "C" __declspec(dllexport)
bool __stdcall GetToolsBlueData()
{
    for each (Form ^ form in Application::OpenForms)
    {
        ToolsForm^ toolsForm = dynamic_cast<ToolsForm^>(form);

        if (toolsForm != nullptr)
        {
            return toolsForm->checkBoxBlue->Checked;
        }
    }

    return false;
}

// Show the specular lighting form
extern "C" __declspec(dllexport)
void __stdcall ShowSpecularForm()
{
    SpecularForm^ form = gcnew SpecularForm();
    form->Show();
}

// Get the specular power slider value
extern "C" __declspec(dllexport)
int __stdcall GetSpecularPower()
{
    for each (Form ^ form in Application::OpenForms)
    {
        SpecularForm^ specularForm = dynamic_cast<SpecularForm^>(form);

        if (specularForm != nullptr)
        {
            return specularForm->trackBarSpecular->Value;
        }
    }

    return 4;
}
