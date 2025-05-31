#include "engine/thread/ThreadManager.h"
#include "engine/core/SceneGraph.h"
#include "InternalOpenGLError.hpp"

engine::ThreadManager* engine::ThreadManager::m_instance = nullptr;
std::once_flag			engine::ThreadManager::m_instanceCreatedFlag;

void engine::ThreadManager::Startup(uint32 numThreads)
{
    // Don't create more threads if there are already existing threads
    if (!GetInstance()->m_workers.empty())
        return;

    // Launch threads
    for (uint32 thread = 0; thread < numThreads; ++thread)
        GetInstance()->m_workers.emplace_back(&ThreadManager::ThreadLoop, GetInstance());
}

void engine::ThreadManager::Shutdown(void)
{
    SynchronizeGameThread(nullptr);
    ExecuteRenderThreadTasks();

    // lock mutex here, we want
    // the bool to be assigned BEFORE
    // notify_all is called, or else threads
    // will be unable to join
    GetInstance()->m_poolMutex.lock();
    GetInstance()->m_stopThreads = true;
    GetInstance()->m_poolMutex.unlock();

    GetInstance()->m_conditionVariable.notify_all();

    for (std::thread& thread : GetInstance()->m_workers)
        thread.join();

    GetInstance()->m_workers.clear();

    delete m_instance;
}

void engine::ThreadManager::UpdateGameLogic(SceneGraph* scene, f32 deltaTime)
{
    if (!scene)
        return;

    // Store a future so that we can wait for the worker thread to finish updating
    GetInstance()->m_gameUpdateFinished = AddTaskWithResult([scene, deltaTime]()
        {
            scene->UpdateComponents<Script>(deltaTime);
        });
}

void engine::ThreadManager::SynchronizeGameThread(SceneGraph* scene)
{ 
    // Wait until the logic update is finished (unless no update was sent to a thread)
    if (GetInstance()->m_gameUpdateFinished.valid())
        GetInstance()->m_gameUpdateFinished.get();

    // Copy updated transforms into separate array for rendering
    if (scene)
        scene->CacheComponents();
}

void engine::ThreadManager::TickAnimations(SceneGraph* scene, f32 deltaTime)
{
    GetInstance()->m_animationUpdateFinished =
        AddTaskWithResult([scene, deltaTime]() {scene->UpdateAnimators(deltaTime); });
}

void engine::ThreadManager::RenderScene(SceneGraph* scene)
{
    ExecuteRenderThreadTasks();

    SynchronizeAnimationThread();

    if (scene)
        scene->RenderFromCache();
}

void engine::ThreadManager::ExecuteRenderThreadTasks(void)
{
    ThreadManager* instance = GetInstance();
    TaskQueue& tasks = instance->GetQueue<ETaskType::GRAPHICS>();

    instance->m_poolMutex.lock();

    while (!tasks.empty())
    {
        std::function<void()>& function = tasks.front();

        function();
        tasks.pop();
        OpenGLError();
    }

    instance->m_poolMutex.unlock();
}

void engine::ThreadManager::SynchronizeAnimationThread(void)
{
    if (GetInstance()->m_animationUpdateFinished.valid())
        GetInstance()->m_animationUpdateFinished.get();
}

void engine::ThreadManager::ThreadLoop(void)
{
    while (true)
    {
        // Define variable outside of block, so it can be executed after
        // lock releases mutex
        std::function<void()> task;

        // Add scope so lock releases when exiting this block
        {
            // lock for condition variable
            std::unique_lock lock(m_poolMutex);

            m_conditionVariable.wait(lock,
            [this]() -> bool {return m_stopThreads || !m_tasks.empty(); });

            // Only break when Shutdown was called AND all tasks are
            // completed. Otherwise the packaged_task pointers won't be be deleted and leak
            if (m_stopThreads && m_tasks.empty())
                break;

            // Exit this block (unlock mutex) and go back to sleep if no work is available and
            // thread is not supposed to join yet
            else if (m_tasks.empty())
                continue;

            task = m_tasks.front();
            m_tasks.pop();
        }

        // Run anonymous function that contains the packaged task ptr
        // and the delete statement
        task();
    }
}

engine::ThreadManager* engine::ThreadManager::GetInstance(void)
{
    // Only create instance once. call_once should be a blocking call, saving the need for double checking
    if (!m_instance)
        std::call_once(m_instanceCreatedFlag, []() {m_instance = new ThreadManager(); });

    return m_instance;
}
