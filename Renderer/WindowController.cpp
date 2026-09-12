#include "WindowController.h" // Load the WindowController header file so we can implement the functions declared in it


WindowController::WindowController() // Constructor: runs when WindowController is first created
{
    m_windowMode = WindowMode::WINDOWED; // Start in normal window mode
    m_wClassName = L"DirectX12"; // Internal name Windows uses for our window
    m_wTitle = L"DirectX12 Renderer"; // Text shown at the top of the window
    m_wClass = {}; // Start window settings empty
    m_hwnd = nullptr; // We don't have a real window yet
    m_windowPosX = 0; // Starting X position
    m_windowPosY = 0; // Starting Y position
    m_windowWidth = 0; // Window width starts at 0
    m_windowHeight = 0; // Window height starts at 0
    m_monitorWidth = 0; // Monitor width not known yet
    m_monitorHeight = 0; // Monitor height not known yet
}



void WindowController::Create(HINSTANCE _hInstance, int _showWnd, WindowMode _mode) // Main function used to create our window
{
    RegClass(_hInstance); // Tell Windows about our window type
    Initialize(_hInstance, _showWnd, _mode); // Actually create and show the window
}



void WindowController::RegClass(HINSTANCE _hInstance) // Register our window settings with Windows
{
    m_wClass.cbSize = sizeof(WNDCLASSEX); // Tell Windows how big this settings object is
    m_wClass.style = WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS; // Basic rules/styles for the window
    m_wClass.lpfnWndProc = WndProc; // Use WndProc to handle window events
    m_wClass.cbClsExtra = 0; // No extra class memory
    m_wClass.cbWndExtra = 0; // No extra window memory
    m_wClass.hInstance = _hInstance; // Tell Windows which app owns this window
    m_wClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION); // Use default app icon
    m_wClass.hCursor = LoadCursor(nullptr, IDC_ARROW); // Use normal arrow mouse cursor
    m_wClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2); // Set a basic background color
    m_wClass.lpszMenuName = nullptr; // No menu bar
    m_wClass.lpszClassName = m_wClassName; // Give Windows our class name
    m_wClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION); // Small version of default icon
    M_ASSERT(RegisterClassEx(&m_wClass), "Failed to register the window class."); // Register it, or stop if it fails
}



void WindowController::GetPrimaryMonitorResolution() // Find the size of the main monitor
{
    M_ASSERT(SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE) == S_OK,
        "Failed to set process DPI awareness.");// Make window sizing work properly with Windows scaling
    const POINT ptZero = { 0, 0 }; // Make a point at screen position 0,0
    HMONITOR primMon = MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY); // Get the main monitor
    M_ASSERT(primMon != nullptr, "Failed tp retrieve primary monitor."); // Make sure we actually found a monitor
    MONITORINFO monitorInfo{ sizeof(MONITORINFO) }; // Create a place to store monitor information
    M_ASSERT(GetMonitorInfo(primMon, &monitorInfo) == TRUE, "Failed to get monitor info."); // Ask Windows for the monitor details
    m_monitorWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left; // Calculate monitor width
    m_monitorHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top; // Calculate monitor height
    m_windowWidth = m_monitorWidth; // For now make window width equal monitor width
    m_windowHeight = m_monitorHeight; // For now make window height equal monitor height
}



void WindowController::CenterWindow() // Put the window in the center of the monitor
{
    m_windowPosX = (m_monitorWidth / 2) - (m_windowWidth / 2); // Find the center X position
    m_windowPosY = (m_monitorHeight / 2) - (m_windowHeight / 2); // Find the center Y position
}



void WindowController::DefaultSize() // Give our normal window a default size
{
    m_windowWidth = m_monitorWidth / 2; // Window = half monitor width
    m_windowHeight = m_monitorHeight / 2; // Window = half monitor height
    CenterWindow(); // Put that window in the middle
}



void WindowController::Initialize(HINSTANCE _hInstance, int _showWnd, WindowMode _mode) // Set up the requested window mode and create it
{
    DWORD style = 0; // Stores which Windows style we want
    GetPrimaryMonitorResolution(); // Find screen size first
    switch (_mode) // Check which window mode was requested
    {
    case WindowMode::WINDOWED: // If normal window was requested
        style = WS_OVERLAPPEDWINDOW; // Use normal borders, title bar, resize buttons, etc
        DefaultSize(); // Make it half-screen size and centered
        break; // Finish this case
    case WindowMode::WINDOWED_BORDERLESS:// Borderless window
        style = WS_POPUP; // Remove normal window borders/title bar
        DefaultSize(); // Half-screen size and centered
        break;
    case WindowMode::WINDOWED_BORDERLESS_FULLSCREEN: // Borderless fullscreen
        style = WS_POPUP; // No borders
        m_windowPosX = 0; // Start at far left
        m_windowPosY = 0; // Start at top
        break;
    case WindowMode::FULLSCREEN_EXCLUSIVE: // Real fullscreen mode
        M_ASSERT(false, "Fullscreen exclusive mode is not implemented yet."); // Force an error because we haven't made this yet
        break;
    }
	// Create the window with the requested settings
    m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_wClassName, m_wTitle, style, m_windowPosX, m_windowPosY,
        m_windowWidth, m_windowHeight, nullptr, nullptr, _hInstance, nullptr);

    M_ASSERT(m_hwnd != nullptr, "Failed to create window."); // Make sure creation worked or stop if it didn't
    ShowWindow(m_hwnd, _showWnd); // Make the window visible
    M_ASSERT(UpdateWindow(m_hwnd), "Failed to update window."); // Draw/update it, or show error if it fails
}


LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam) // Function that handles Windows events
{
    bool processed = true; // Assume we're handling the event ourselves
    switch (_message) // Check what happened
    {
    case WM_DESTROY: // Window was closed/destroyed
        PostQuitMessage(0); // Tell the program to quit
        return 0; // Event handled
    case WM_KEYUP: // A keyboard key was released
        if (_wParam == VK_ESCAPE) // Was it the Escape key?
        {
            PostQuitMessage(0); // Quit the program
            return 0; // Event handled
        }
        break;
    default: // Some event we didn't handle
        processed = false; // Tell ourselves Windows should handle it
        break;
    }
    if (!processed)// If we didn't handle the event
    {
        return DefWindowProc(_hWnd, _message, _wParam, _lParam); // Let Windows handle it normally
    }

}

