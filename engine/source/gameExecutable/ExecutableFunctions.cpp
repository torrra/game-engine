#include "engine/Engine.h"
#include "engine/ConsoleLog.hpp"

#include <filesystem>
#include <iostream>

#undef ERROR
#include "gameExecutable/ExecutableFunctions.h"

void* InitializeEngine(void)
{
    for (auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
    {
        if (!std::filesystem::exists(entry))
            continue;

        if (!std::filesystem::is_regular_file(entry) || !entry.path().has_extension())
            continue;

        const std::filesystem::path& filePath = entry.path();
        uint64 size = filePath.native().size();
        std::cout << "Directory entry: " << filePath << '\n';

        constexpr wchar_t extension[] = L".mustang";
        constexpr uint64 arrayLength = sizeof(extension) / sizeof(extension[0]) - 1;

        if (size <= sizeof(extension))
            continue;

        uint64 dotPos = size - arrayLength;

        if (memcmp(filePath.c_str() + dotPos, extension, sizeof(extension)) == 0)
        {
            engine::Engine* defaultEngine = new engine::Engine(false);
            defaultEngine->OpenProject(filePath);
            defaultEngine->Startup();
            return defaultEngine;
        }

    }

    engine::PrintLog(engine::ErrorPreset(), "Unable to find project file");
    return nullptr;
}

void RunGame(void*& data)
{
    if (!data)
        return;

    engine::Engine* enginePtr = reinterpret_cast<engine::Engine*>(data);
    engine::Window* window = enginePtr->GetWindow();

    enginePtr->GetCurrentScene().Start();

    while (!window->ShouldWindowClose())
    {
        enginePtr->UpdateGameplay();
        enginePtr->UpdateApplicationWindow();
    }
}

void Cleanup(void*& data)
{
    if (!data)
        return;

    engine::Engine* enginePtr = reinterpret_cast<engine::Engine*>(data);

    enginePtr->ShutDown();

    delete enginePtr;
    data = nullptr;
}