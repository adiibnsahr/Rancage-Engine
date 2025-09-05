// Core/Rancage Engine.cpp

#include "../Platform/Win32/Window.h"
#include "../Core/Utils/Logger.h"

int main()
{
    Logger::Log(Logger::Level::INFO, "Starting Rancage Engine Core...");

    Window window;
    if (!window.Create("Rancage Engine", 1280, 720))
    {
        Logger::Log(Logger::Level::FAILED, "Failed to create window.");
        return -1;
    }

    window.Show();

    while (true)
    {
        window.PollEvents();
        // main loop update here
    }

    return 0;
}