#include "game/GameScene.h"
#include "thread/ThreadManager.h"
#include "core/systems/ScriptSystem.h"
#include "Engine.h"

#include "physics/PhysicsEngine.h"
#include "physics/Raycast.h"

#include <fstream>

namespace engine
{
    GameScene::GameScene(const std::filesystem::path& path, const std::string& name)
        : m_path(path)
    {      
        UpdateNameAndPath(name);
        DeserializeText();
    }

    void GameScene::Start(void)
    {
        m_timeSincePhysicsTick = 0.f;
        //m_time.Reset();
        SerializeText();
        ScriptSystem::ResetState(&m_graph);
        m_graph.StartAllScripts();
        m_state = EGameState::RUNNING;
    }

    void GameScene::Stop(void)
    {
        m_state = EGameState::STOPPED;
    }

    void GameScene::Reset(bool reload)
    {
        m_graph.CleanRigidBodies();
        
        if (reload)
            DeserializeText();
        else
            m_graph = SceneGraph();

        ScriptSystem::ResetState();
    }

    void GameScene::Rename(const std::string& newName)
    {
        if (std::filesystem::exists(m_path))
            std::filesystem::remove(m_path);

        UpdateNameAndPath(newName);
        SerializeText();
    }

    void GameScene::EditPath(const std::filesystem::path& path)
    {
        m_path = path;
        m_path.append(m_name).replace_extension(".mscn");
    }

    void GameScene::LoadNewScene(bool serialize, const std::filesystem::path& path)
    {
        if (serialize)
            SerializeText();

        if (!std::filesystem::exists(path))
            return;

        m_path = path;
        m_name = m_path.filename().replace_extension().string();
        DeserializeText();
        
    }

    void GameScene::SerializeText(void)
    {
        std::ofstream output(m_path, std::ios::out | std::ios::trunc);
        m_graph.SerializeText(output);
    }

    bool GameScene::DeserializeText(void)
    {
        std::ifstream input(m_path, std::ios::in | std::ios::binary);

        if (!input)
            return false;

        m_graph = SceneGraph();
        m_graph.DeserializeText(input);
        PhysicsEngine::Get().StepSimulation(0.0001f);
        m_graph.SyncTransformsPostPhysics();
        ThreadManager::SynchronizeGameThread(&m_graph);
        return true;
    }

    void GameScene::Tick(void)
    {
 

        if (m_state == EGameState::RUNNING)
        {
            ThreadManager::SynchronizeGameThread(&m_graph);
            m_graph.SyncRigidbodiesPrePhysics();
            PhysicsUpdate();
            ThreadManager::UpdateGameLogic(&m_graph, m_time.GetDeltaTime());
        }

        else if (Engine::HasEditor())
        {
            // Update render cache
            ThreadManager::SynchronizeGameThread(&m_graph);

            // Execute a dummy physics tick to update debug draw 
            m_graph.SyncRigidbodiesPrePhysics();
            PhysicsEngine::Get().StepSimulation(0.0001f);

            // 'undo' physics tick (set physx transform back to pre-update values)
            m_graph.SyncRigidbodiesPrePhysics();

            for (RigidBodyDynamic& rigidbody : m_graph.GetComponentArray<RigidBodyDynamic>())
                rigidbody.SetLinearVelocity({ 0.f, 0.f, 0.f });

        }

        m_time.Update();
    }

    SceneGraph* GameScene::GetGraph(void)
    {
        return &m_graph;
    }

    bool GameScene::IsRunning(void) const
    {
        return m_state == EGameState::RUNNING;
    }


    void GameScene::PhysicsUpdate(void)
    {
        constexpr f32 physicsUpdateInterval = 0.02f;

        m_timeSincePhysicsTick += m_time.GetDeltaTime();

        int32 numPhysicsUpdate = static_cast<int32>(m_timeSincePhysicsTick / physicsUpdateInterval);

        if (numPhysicsUpdate < 1)
            return;

        f32 interval = m_timeSincePhysicsTick / static_cast<f32>(numPhysicsUpdate);

        for (int32 updateNum = 0; updateNum < numPhysicsUpdate; ++updateNum)
            PhysicsEngine::Get().StepSimulation(interval);
        
         m_graph.SyncTransformsPostPhysics();
         m_timeSincePhysicsTick = 0.f;
    }

    const Time& GameScene::GetTime(void) const
    {
        return m_time;
    }

    void GameScene::UpdateNameAndPath(const std::string& name)
    {
        m_path.replace_filename(name).replace_extension(".mscn");
        m_name = name;
    }
}