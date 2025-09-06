// Core/Debug/DebugController.cpp
#include "DebugController.h"
#include <Windows.h>

void DebugController::Update() 
{
    // Toggle debug mode with F3 key for example
    if (GetAsyncKeyState(VK_F3) & 1) 
    { 
        // Check for key press edge
        debugEnabled = !debugEnabled;
    }
}
