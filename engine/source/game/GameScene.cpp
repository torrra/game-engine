#include "game/GameScene.h"
#include "thread/ThreadManager.h"
#include "core/systems/ScriptSystem.h"

#include "physics/PhysicsEngine.h"

#include <fstream>
 
namespace engine
{
    GameScene::GameScene(const std::filesystem::path& path, const std::string& name)
        : m_path(path)
    {      
        Rename(name);
        DeserializeText();
    }

    void GameScene::Start(void)
    {

        m_time.Reset();
        SerializeText();
        ScriptSystem::ResetState(&m_graph);
        m_graph.StartAllScripts();
        m_state = EGameState::RUNNING;
    }

    void GameScene::Stop(void)
    {
        m_state = EGameState::STOPPED;
    }

    void GameScene::Reset(void)
    {
        DeserializeText();
        ScriptSystem::ResetState();
    }

    void GameScene::Rename(const std::string& newName)
    {
        m_path.remove_filename();
        m_path.append(newName + ".mscn");
        m_name = newName;
    }

    void GameScene::LoadNewScene(const std::string& /*name*/)
    {
    }

    void GameScene::SerializeText(void)
    {
        std::ofstream output(m_path, std::ios::out | std::ios::trunc);
        m_graph.SerializeText(output);
    }

    bool GameScene::DeserializeText(void)
    {
        if (!std::filesystem::exists(m_path))
            return false;

        std::ifstream input(m_path, std::ios::in | std::ios::binary);

        if (!input)
            return false;

        m_graph = SceneGraph();
        m_graph.DeserializeText(input);
        return true;
    }

    void GameScene::Tick(void)
    {
        if (m_state == EGameState::RUNNING)
        {
            ThreadManager::SynchronizeGameThread(&m_graph);
            PhysicsUpdate();
            ThreadManager::UpdateGameLogic(&m_graph, m_time.GetDeltaTime());
            m_time.Update();
        }
    }

    SceneGraph* GameScene::GetGraph(void)
    {
        return &m_graph;
    }


    void GameScene::PhysicsUpdate(void)
    {
        constexpr f32 physicsUpdateInterval = 1.f / 0.2f;

        m_timeSincePhysicsTick += m_time.GetDeltaTime();
        int32 numPhysicsUpdate = static_cast<int32>(m_timeSincePhysicsTick * physicsUpdateInterval);

        if (numPhysicsUpdate < 1)
            return;

        f32 interval = m_timeSincePhysicsTick / static_cast<f32>(numPhysicsUpdate);

        m_graph.SyncRigidbodiesPrePhysics();

        for (int32 updateNum = 0; updateNum < numPhysicsUpdate; ++updateNum)
            PhysicsEngine::Get().StepSimulation(interval);

         m_graph.SyncTransformsPostPhysics();
         m_timeSincePhysicsTick = 0.f;
    }
}