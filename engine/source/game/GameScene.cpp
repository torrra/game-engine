#include "game/GameScene.h"
#include "resource/ResourceManager.h"
#include "thread/ThreadManager.h"
#include "core/systems/ScriptSystem.h"

#include "physics/PhysicsEngine.h"

#include <fstream>
 
namespace engine
{
    GameScene::GameScene(const std::filesystem::path& path, const std::string& name)
        : m_name(name), m_path(path)
    {      
            DeserializeText();
    }

    void GameScene::Start(bool withEditor)
    {
        SerializeText();
        ScriptSystem::ResetState(&m_graph);
        m_graph.RegisterAllEntities();
        m_graph.RegisterAllComponents();
        m_graph.StartAllScripts();

        if (withEditor)
            ThreadManager::AddTask(&GameScene::EditorUpdateLoop, this);

        else
            ThreadManager::AddTask(&GameScene::StandaloneLoop, this);
    }

    void GameScene::Rename(const std::string& newName)
    {
        m_path.remove_filename();
        m_path.append(newName);
        m_name = newName;
    }

    void GameScene::LoadNewScene(const std::string& /*name*/)
    {
    }

    void GameScene::SerializeText(void)
    {
        std::ofstream output(m_path, std::ios::out);
        m_graph.SerializeText(output);
    }

    bool GameScene::DeserializeText(void)
    {
        if (!std::filesystem::exists(m_path))
            return false;

        std::ifstream input(m_path, std::ios::in | std::ios::binary);
        SceneGraph tempOldState;

        tempOldState.DeserializeText(input);
        m_graph = std::move(tempOldState);

        return true;

    }

    void GameScene::EditorUpdateLoop(void)
    {
        // Copy data from last frame into render caches
        ThreadManager::SynchronizeGameThread(&m_graph);

        PhysicsUpdate();
        ThreadManager::UpdateGameLogic(&m_graph, m_time.GetDeltaTime());
    }

    void GameScene::StepGameplayUpdate(void)
    {
        //m_graph.UpdateComponents<Transform>()
    }

    void GameScene::PhysicsUpdate(void)
    {
        constexpr f32 physicsUpdateInterval = 1.f / 0.2f;

        int32 numPhysicsUpdate = static_cast<int32>(m_timeSincePhysicsTick += m_time.GetDeltaTime() * physicsUpdateInterval);

        for (int32 updateNum = 0; updateNum < numPhysicsUpdate; ++updateNum)
        {
            m_graph.SyncRigidbodiesPrePhysics();
            PhysicsEngine::Get().StepSimulation(m_time.GetDeltaTime());
            m_graph.SyncTransformsPostPhysics();
            m_timeSincePhysicsTick = 0.f;
        }
    }

    void GameScene::StandaloneLoop(void)
    {
    }
}