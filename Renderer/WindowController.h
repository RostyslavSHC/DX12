#pragma once // Load this file only once

#pragma comment(lib, "shcore.lib") // (Link shcore.lib for SetProcessDpiAwareness) Connect Windows DPI library

#include <shellscalingapi.h> // For SetProcessDpiAwareness (Gives us DPI scaling functions)

#include "StandardIncludes.h" // Load common Windows/helper code


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam); // Function that receives Windows events like close, keyboard, etc



enum class WindowMode
    // List of possible window modes
{
    WINDOWED, // Normal window with borders
    WINDOWED_BORDERLESS, // Window without borders
    WINDOWED_BORDERLESS_FULLSCREEN, // Borderless window filling the whole screen
    FULLSCREEN_EXCLUSIVE // Real fullscreen mode, not implemented yet
};



// Singleton<WindowController> helps make sure there is a single shared instance that every part of the game can access
// So instead of accidentally creating: WindowController window1; and WindowController window2; you use the same one everywhere, something like: WindowController::Instance()
class WindowController : public Singleton<WindowController> // Create WindowController and make it a Singleton
{
public:
    // Constructor/Destructor
    WindowController(); // Constructor, runs when controller is created
    virtual ~WindowController() {} // Destructor, runs when controller is destroyed

    // Accessors
    HWND GetHWND() { return m_hwnd; } // Get the window handle
    int GetWindowWidth() const { return m_windowWidth; } // Get the window width
	int GetWindowHeight() const { return m_windowHeight; } // Get the window height
    bool* GetKeyStates() { return m_keyStates; } // Get the keyboard state


    // Methods
    void Create(HINSTANCE _hInstance, int _showWnd, WindowMode _mode); // Main function that creates the window

private:
	// Methods
    void RegClass(HINSTANCE _hInstance); // Register the window type with Windows
    void GetPrimaryMonitorResolution(); // Find monitor width and height
    void CenterWindow(); // Put the window in the middle of screen
    void DefaultSize(); // Set default window size
    void Initialize(HINSTANCE _hInstance, int _showWnd, WindowMode _mode); // Actually set up/create the window


    LPCWSTR m_wClassName;// Window class name
    LPCWSTR m_wTitle;// Text shown in window title bar
    WNDCLASSEX m_wClass;// Stores Windows window-class settings
    HWND m_hwnd;// Handle/reference to our actual window
    WindowMode m_windowMode;// Current window mode
    int m_windowPosX; // Window X position
    int m_windowPosY; // Window Y position
    int m_windowWidth; // Window width
    int m_windowHeight; // Window height
    int m_monitorWidth; // Monitor width
    int m_monitorHeight; // Monitor height
    bool m_keyStates[256]; // Keyboard state
};