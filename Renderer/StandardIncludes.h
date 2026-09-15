#pragma once // Only include this header file once

#define WIN32_LEAN_AND_MEAN // Load a smaller/basic version of Windows.h
#define NOMINMAX            // Stops Windows from creating its own min/max macros

#include <Windows.h>   // Gives us Windows functions like GetLastError()
#include <algorithm>   // Gives us useful C++ algorithms like sort(), min(), max(), etc
#include <wrl.h>       // Gives us the Windows Runtime Library


// DirectX12
#include <d3d12.h>     // Direct3D 12
#include <dxgi1_6.h>   // DirectX Graphics Infrastructure 1.6

#include "Singleton.h" // Includes our Singleton class
#include "include/d3dx12/d3dx12.h" // Direct3D 12 helper library

using Microsoft::WRL::ComPtr; // ComPtr is a smart pointer for COM objects
using namespace std; // So we don't have to use std::




// Creates our own assert/check called M_ASSERT
    // _cond = condition to check, _msg = error message
    // If the condition is FALSE...
    // Print our error message in the debugger
    // Print the Windows error too
    // Immediately stop the program
#define M_ASSERT(_cond, _msg) \
    if (!(_cond)) { OutputDebugStringA(_msg); OutputDebugStringA(GetError()); std::abort(); \
}

static LPVOID lpErrorMessage; // Variable that will hold the Windows error message

static char* GetError() // Function that gets the latest Windows error as text
{
    DWORD dwErrorCode = GetLastError(); // Get the number/code of the last Windows error

    FormatMessage( // Convert that error number into readable text
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, dwErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpErrorMessage, 0, nullptr                          // No extra arguments needed
    );

    return static_cast<char*>(lpErrorMessage); // Convert it to char* and return the text
};