#pragma once 

#include <filesystem>
#include <string>
#include <atomic>

#include "engine/core/SceneGraph.h"
#include "engine/utility/Timer.h"

namespace engine
{
    enum class EGameState : uint8
    {
        STOPPED,
        RUNNING,
    };

    class GameScene
    {
    public:

        ENGINE_API GameScene(void) = default;
        ENGINE_API GameScene(const std::filesystem::path& path, const std::string& name);
        GameScene(const GameScene&) = delete;
        ENGINE_API GameScene(GameScene&&) = default;
        ENGINE_API ~GameScene(void) = default;

        ENGINE_API void Start(void);

        ENGINE_API void Tick(void);

        ENGINE_API void Stop(void);

        ENGINE_API void Reset(void);

        ENGINE_API  void Rename(const std::string& newName);
        ENGINE_API void EditPath(const std::filesystem::path& path);

        ENGINE_API void LoadNewScene(bool serialize, const std::filesystem::path& path);

        ENGINE_API void SerializeText(void);

        ENGINE_API bool DeserializeText(void);

        ENGINE_API SceneGraph* GetGraph(void);

        ENGINE_API GameScene& operator=(GameScene&&) = default;


    private:

        void PhysicsUpdate(void);
        void UpdateNameAndPath(const std::string& name);

        std::filesystem::path       m_path;
        std::string                 m_name;
        SceneGraph                  m_graph;
        Time                        m_time;
        f32                         m_timeScale = 1.f;
        f32                         m_timeSincePhysicsTick = 0.f;
        std::atomic<EGameState>     m_state = EGameState::STOPPED;
    };
}