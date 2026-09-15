#include "StandardIncludes.h" // Load our common Windows/helper code
#include "WindowController.h" // Load the code that creates and controls the window

#include "Renderer.h"
#include "iostream"



int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) // Main function - program starts here
{
    WindowController* wc = &WindowController::GetInstance(); // Get the one shared WindowController and save its address in wc
    wc->Create(hInstance, nShowCmd, WindowMode::WINDOWED); // Create our app in normal windowed mode

    Renderer* r = &Renderer::GetInstance(); // Get the one shared Renderer and save its address in r
    r->ConfigurePipeline(false, wc->GetHWND(), wc->GetWindowWidth(), wc->GetWindowHeight()); // Call the renderer to configure the pipeline


    // SOme temporary debug output
    DXGI_SWAP_CHAIN_DESC scDesc;
    r->GetSwapChain().Get()->GetDesc(&scDesc);

    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = r->GetRTVHeap().Get()->GetDesc(); // Make an empty descriptor heap description
    std::cout << "Window Controller: Window = " << wc->GetHWND() << "\n" <<
        "Swapchain: Buffer count = " << to_string(scDesc.BufferCount) <<
        "; Window = " << scDesc.OutputWindow << "\n" <<
        "RTV Heap: Number of descriptors = " << heapDesc.NumDescriptors;






// Main message loop
    MSG msg = {}; // Make an empty variable that will hold Windows messages/events

    while (msg.message != WM_QUIT) // Keep getting events until the program is told to quit
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Is there a message waiting?
        {
            TranslateMessage(&msg); // Convert keyboard input into easier-to-use messages
            DispatchMessage(&msg); // Send the event to WndProc to handle it
        }
    }
}


