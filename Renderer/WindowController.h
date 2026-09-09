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




class WindowController : public Singleton<WindowController> // Create WindowController and make it a Singleton
{
public:
    WindowController(); // Constructor, runs when controller is created
    virtual ~WindowController() {} // Destructor, runs when controller is destroyed
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
};