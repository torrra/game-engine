#pragma once 

#include <filesystem>
#include <string>
#include <future>

#include "engine/core/SceneGraph.h"
#include "engine/utility/Timer.h"

namespace engine
{
    enum class EGameState : uint8
    {
        STOPPED,
        RUNNING,
        PAUSED
    };

    class GameScene
    {
    public:

        GameScene(void) = delete;
        GameScene(const std::filesystem::path& path, const std::string& name);
        GameScene(GameScene&&) = default;
        ~GameScene(void) = default;

        void Start(bool withEditor = false);
        void Rename(const std::string& newName);
        void LoadNewScene(const std::string& name);

        void SerializeText(void);
        bool DeserializeText(void);

    private:

        void EditorUpdateLoop(void);
        void StepGameplayUpdate(void);
        void PhysicsUpdate(void);
        void StandaloneLoop(void);

        std::filesystem::path   m_path;
        std::string             m_name;
        SceneGraph              m_graph;
        Time                    m_time;
        f32                     m_timeScale = 1.f;
        f32                     m_timeSincePhysicsTick = 0.f;
    };
}