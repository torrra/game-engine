#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <memory>
#include <queue>
#include <type_traits>
#include <utility>
#include <vector>

#include "engine/CoreTypes.h"
#include "engine/EngineExport.h"

#define DEFAULT_NUM_THREADS ((std::thread::hardware_concurrency() / 4) * 3)

namespace engine
{
	class ThreadManager
	{
	public:

		// Add a function to the task list, without getting a future for its return value
		template <typename TFunctionType, typename... TVariadicArgs>
		static void AddTask(TFunctionType&& function, TVariadicArgs&&... args);

		// Add a function to the task list, and get a future for its return value
		// Returns a std::future that contains the return type of the function
		// passed as 1st argument, which will be deduced at compile time:
		// std::future<std::invoke_result_t<TFunctionType, TVariadicArgs...>>
		template <typename TFunctionType, typename... TVariadicArgs> [[nodiscard]]
		static auto AddTaskWithResult(TFunctionType&& function, TVariadicArgs&&... args);

		// Create threads
		// NOTE: Lua also uses its own thread, and the physics engine also uses threads,

		ENGINE_API
		static void Startup(uint32 numThreads = DEFAULT_NUM_THREADS);

		// Finish existing tasks and join all threads
		ENGINE_API
		static void Shutdown(void);

		// Update game-related components on another thread
		// As of now, only scripts are directly updated, though scripts can
		// affect transform and camera components. 
		// Call SynchronizeGameThread() before this function to be able to 
		// parallelize rendering
		ENGINE_API
		static void UpdateGameLogic(class SceneGraph* scene, f32 deltaTime);
		
		// Wait until tick update is finished and copy components necessary for
		// rendering into a separate cache
		ENGINE_API
		static void SynchronizeGameThread(class SceneGraph* scene = nullptr);
		
		// Render active scene using its previously cached values.
		// Should be optimally called after SynchronizeGameThread() and
		// UpdateGameLogic() as this function will begin rendering on this thread
		ENGINE_API
		static void RenderScene(class SceneGraph* scene);

	private:

		ThreadManager(void) = default;
		ThreadManager(ThreadManager&&) = delete;
		~ThreadManager(void) = default;

		// Loop executed wby worker threads to sleep until
		// work is available and execute tasks
		void TreadLoop(void);


		// Export this function as it will be called from inline functions
		ENGINE_API
		static ThreadManager* GetInstance(void);
		
		std::mutex							m_poolMutex;
		std::condition_variable				m_conditionVariable;
		std::queue<std::function<void()>>	m_tasks;
		std::vector<std::thread>			m_workers;
		std::future<void>					m_gameUpdateFinished;
		bool								m_stopThreads = false;

		static std::once_flag				m_instanceCreatedFlag;
		static ThreadManager*				m_instance;
		  
	};


	template<typename TFunctionType, typename ...TVariadicArgs>
	inline void ThreadManager::AddTask(TFunctionType&& function, TVariadicArgs && ...args)
	{
		// std::bind will fail to compile if function is not invocable, but
		// this assert will give a much clearer error message on failure
		static_assert(std::is_invocable_v<TFunctionType, TVariadicArgs...>,
					 "Argument 'function' is not invocable");

		std::function<void()> packagedFunc = std::bind(std::forward<TFunctionType>(function),
													   std::forward<TVariadicArgs>(args)...);

		// We don't want another thread to touch the queue while we add a task
		GetInstance()->m_poolMutex.lock();
		GetInstance()->m_tasks.push(packagedFunc);
		GetInstance()->m_poolMutex.unlock();
		GetInstance()->m_conditionVariable.notify_one();
	}


	template<typename TFunctionType, typename ...TVariadicArgs>
	inline auto ThreadManager::AddTaskWithResult(TFunctionType&& function, TVariadicArgs&& ...args)
	{
		// std::bind will fail to compile if function is not invocable, but
		// this assert will give a much clearer error message on failure
		static_assert(std::is_invocable_v<TFunctionType, TVariadicArgs...>,
				      "Argument 'function' is not invocable");

		// 'function' return type
		using TReturnType = std::invoke_result_t<TFunctionType, TVariadicArgs...>;

		// Packaged task type, with the function's declared type as template argument
		using TTaskType = std::packaged_task<decltype(function(args...))()>;

		// std::bind return value type is unspecified, see:
		// https://en.cppreference.com/w/cpp/utility/functional/bind
		auto packagedFunc = std::bind(std::forward<TFunctionType>(function),
									  std::forward<TVariadicArgs>(args)...);

		// Create pointer to packaged task so that a lambda can capture it
		// regardless of its return value and parameters
		// MUST be manually deallocated after execution
		TTaskType* task = new TTaskType(packagedFunc);
		std::future<TReturnType> future = task->get_future();

		// lock to ensure that only this thread can write into the queue
		GetInstance()->m_poolMutex.lock();

		// use lambda so that the task is stored as a void() function
		GetInstance()->m_tasks.push([task]() -> void {(*task)(); delete task; });
		GetInstance()->m_poolMutex.unlock();
		GetInstance()->m_conditionVariable.notify_one();

		return future;
	}

}