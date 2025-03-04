#pragma once

#include "Resource.h"

#include <mutex>
#include <string>
#include <unordered_map>

#include "engine/utility/MemoryCheck.h"
#include "engine/EngineExport.h"

/*
*	-------- Resource Manager --------
*	- Load resource					DONE
*	- Get resource members			DONE
*	- Unload resource				DONE
*	- check memory leak				DONE
*	- Clean up resource manager		DONE
*	- Multi-threading
*	- Meta data
*	----------------------------------
*/

namespace engine
{
	class ResourceManager
	{
	public:
		template<typename T>
		static void					Load(std::string const& fileName);
		template<typename T>
		static T*					GetResource(std::string const& fileName);
		ENGINE_API static void		Unload(std::string const& fileName);
		ENGINE_API static void		UnloadAll(void);
		ENGINE_API static void		CloseResourceManager(void);

	private:
									ResourceManager(void) = default;
									ResourceManager(ResourceManager const& rManager) = delete;
		ResourceManager&			operator=(ResourceManager const& rManager) = delete;

		ENGINE_API static ResourceManager*		GetInstance(void);
		ENGINE_API static bool					HasResource(std::string const& fileName);

		static std::mutex			m_mutex;
		static ResourceManager*		m_instance;

		std::unordered_map<std::string, IResource*> m_resources;
	};

	// Template function definitions
	template<typename T>
	inline void engine::ResourceManager::Load(std::string const& fileName)
	{
		// Check if resource exists
		if (HasResource(fileName))
		{
			std::printf("Resource already loaded\n");
			return;
		}

		GetInstance()->m_resources[fileName] = new T();
		GetInstance()->m_resources[fileName]->LoadResource(fileName.c_str());
	}

	template<typename T>
	inline T* engine::ResourceManager::GetResource(std::string const& fileName)
	{
		// Check if resource exists
		if (!HasResource(fileName))
			return nullptr;

		return dynamic_cast<T*>(GetInstance()->m_resources[fileName]);
	}
}