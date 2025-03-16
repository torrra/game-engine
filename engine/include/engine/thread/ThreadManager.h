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

namespace engine
{
	class ThreadManager
	{
	public:

		ENGINE_API
		ThreadManager(void) = default;
		
		ENGINE_API
		~ThreadManager(void) = default;

		ThreadManager(ThreadManager&&) = delete;

		// Add a function to the task list, without getting a future for its return value
		template <typename TFunctionType, typename... TVariadicArgs>
		void AddTask(TFunctionType&& function, TVariadicArgs&&... args);

		// Add a function to the task list, and get a future for its return value
		// Returns a std::future that contains the return type of the function
		// passed as 1st argument, which will be deduced at compile time:
		// std::future<std::invoke_result_t<TFunctionType, TVariadicArgs...>>
		template <typename TFunctionType, typename... TVariadicArgs> [[nodiscard]]
		auto AddTaskWithResult(TFunctionType&& function, TVariadicArgs&&... args);

		// Create threads
		ENGINE_API
		void Startup(uint32 numThreads = std::thread::hardware_concurrency() - 2);

		// Finish existing tasks and join all threads
		ENGINE_API
		void Shutdown(void);

	private:

		// Loop executed wby worker threads to sleep until
		// work is available and execute tasks
		void TreadLoop(void);
		
		std::mutex							m_poolMutex;
		std::condition_variable				m_conditionVariable;
		std::vector<std::thread>			m_workers;
		std::queue<std::function<void()>>	m_tasks;
		bool								m_stopThreads = false;
		  
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
		m_poolMutex.lock();
		m_tasks.push(packagedFunc);
		m_poolMutex.unlock();
		m_conditionVariable.notify_one();
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
		m_poolMutex.lock();

		// use lambda so that the task is stored as a void() function
		m_tasks.push([task]() -> void {(*task)(); delete task; });
		m_poolMutex.unlock();
		m_conditionVariable.notify_one();

		return future;
	}

}