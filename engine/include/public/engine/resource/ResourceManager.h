#pragma once

#include "Resource.h"

#include <mutex>
#include <string>
#include <unordered_map>

//#include "engine/utility/MemoryCheck.h"
#include "engine/EngineExport.h"

/*
*	-------- Resource Manager --------
*	- Meta data
*	----------------------------------
*/

namespace engine
{
	class ResourceManager
	{
	public:
		template<typename TResourceType>
		static void					Load(std::string const& fileName);
		ENGINE_API static void		LoadShader(
										const char* shaderProgramName, 
										const char* vertShader, 
										const char* fragShader);

        // Create a resource from data that already exists in memory instead
        // of loading a file from disk
        template <typename TResourceType, typename... TVariadicArgs>
        static void CreateFromData(const std::string& name, TVariadicArgs&&... args);

		template<typename TResourceType>
		static const TResourceType*	GetResource(std::string const& fileName);
		ENGINE_API static void		Unload(std::string const& fileName);
		ENGINE_API static void		UnloadAll(void);
		ENGINE_API static void		ShutDown(void);

		ENGINE_API static const std::string* FindKeyByVal(const IResource* resource);

	private:
									ResourceManager(void) = default;
									ResourceManager(ResourceManager const& rManager) = delete;
		ResourceManager&			operator=(ResourceManager const& rManager) = delete;

		ENGINE_API static ResourceManager*	GetInstance(void);
		ENGINE_API static bool				HasResource(std::string const& fileName);

		ENGINE_API
        static std::mutex			m_mutex;
        static ResourceManager* m_instance;

		std::unordered_map<std::string, IResource*> m_resources;
	};

	// Template function definitions
	template<typename TResourceType>
	inline void engine::ResourceManager::Load(std::string const& fileName)
	{
        TResourceType* newVal = nullptr;

        {
            std::lock_guard lock(m_mutex);

            // Check if resource exists
            if (HasResource(fileName))
            {
                //std::printf("Resource '%s' already loaded\n", fileName.c_str());
                return;
            }

            newVal = new TResourceType();
            GetInstance()->m_resources[fileName] = newVal;
        }

        if (!newVal->LoadResource(fileName.c_str()))
        {
            m_mutex.lock();
            GetInstance()->m_resources.erase(fileName);
            delete newVal;
            m_mutex.unlock();
        }
	}

    template<typename TResourceType, typename ...TVariadicArgs>
    inline void ResourceManager::CreateFromData(const std::string& name, TVariadicArgs && ...args)
    {
        std::lock_guard lock(m_mutex);
        
        // Check if resource exists
        if (HasResource(name))
        {
            std::printf("Resource '%s' already loaded\n", name.c_str());
            return;
        }
        
        TResourceType* newVal = new TResourceType(std::forward<TVariadicArgs>(args)...);
        GetInstance()->m_resources[name] = newVal;
    }

    template<typename TResourceType>
	inline const TResourceType* engine::ResourceManager::GetResource(std::string const& fileName)
	{
		// Check if resource exists
		if (!HasResource(fileName))
			return nullptr;


        if constexpr (!IsLoadedAsync<TResourceType>::m_value)
            return dynamic_cast<TResourceType*>(GetInstance()->m_resources[fileName]);

        else
        {
            TResourceType* resource = dynamic_cast<TResourceType*>(GetInstance()->m_resources[fileName]);

            if (!resource->HasFailedToLoad())
                return resource;

            Unload(fileName);
            return nullptr;
        }
	}
}