#include "StandardIncludes.h" // Load our common Windows/helper code
#include "WindowController.h" // Load the code that creates and controls the window

#include "Renderer.h"
#include "iostream"



int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) // Main function - program starts here
{
    WindowController* wc = &WindowController::GetInstance(); // Get the one shared WindowController and save its address in wc
    wc->Create(hInstance, nShowCmd, WindowMode::WINDOWED); // Create our app in normal windowed mode

    Renderer* r = &Renderer::GetInstance(); // Get the one shared Renderer and save its address in r
    r->ConfigurePipeline(false); // Call the function that configures the pipeline
    const char *byte_addr = reinterpret_cast<const char*>(r->GetDevice().Get()); // Get the address of the device
    std::cout << static_cast<const void*>(byte_addr); // Print the address of the device

// Main message loop
    MSG msg = {}; // Make an empty variable that will hold Windows messages/events

    while (GetMessage(&msg, nullptr, 0, 0)) // Keep getting events until the program is told to quit
    {
        TranslateMessage(&msg); // Convert keyboard input into easier-to-use messages
        DispatchMessage(&msg); // Send the event to WndProc to handle it
    }
}



