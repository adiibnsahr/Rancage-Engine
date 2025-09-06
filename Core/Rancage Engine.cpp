// Core/Main.cpp
#include "Platform/Win32/Window.h"
#include "Core/Debug/DebugRenderer.h"
#include "Core/Debug/DebugController.h"
#include "Core/Debug/DebugLogger.h"
#include "Core/Utils/Logger.h"
#include <Windows.h>

DebugRenderer debugRenderer;
DebugController debugController;

int main() {
    Logger::Log(Logger::Level::INFO, "Starting Rancage Engine Core...");

    Window window;
    if (!window.Create("Rancage Engine", 1280, 720)) {
        Logger::Log(Logger::Level::FAILED, "Failed to create window.");
        return -1;
    }
    window.Show();

    debugRenderer.Initialize();
    DebugLogger::Initialize();

    MSG msg = {};
    bool running = true;

    while (running) {
        // Process all pending messages
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (!running) break;

        // Update input / debug controller
        debugController.Update();

        // Begin debug frame
        debugRenderer.BeginFrame();

        if (debugController.IsDebugEnabled()) {
            debugRenderer.DrawAABB(Vector3(-1, -1, -1), Vector3(1, 1, 1), Vector3(1, 0, 0));
            DebugLogger::Log("Debug Mode Active");
        }

        // Submit debug primitives
        debugRenderer.EndFrame();

        // TODO: game update & render
        // Present swapchain, etc.
    }

    debugRenderer.Shutdown();
    return static_cast<int>(msg.wParam);
}
