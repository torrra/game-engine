#pragma once

#include "Resource.h"

#include <mutex>
#include <string>
#include <unordered_map>

//#include "engine/utility/MemoryCheck.h"
#include "engine/EngineExport.h"
#include "engine/Engine.h"

#include "engine/utility/ResourceContainer.h"
//#include "engine/utility/ResourceRefDecl.h"
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
		static void					Load(std::string const& fileName, bool absolute = false);

		ENGINE_API static void		LoadShader(
										const char* shaderProgramName, 
										const char* vertShader, 
										const char* fragShader,
                                        bool isVertAbsolute = false, bool isFragAbsolute = false);

        // Create a resource from data that already exists in memory instead
        // of loading a file from disk
        template <typename TResourceType, typename... TVariadicArgs>
        static void CreateFromData(const std::string& name, TVariadicArgs&&... args);

		template<typename TResourceType>
		static ResourceRef<TResourceType>	GetResource(std::string const& fileName);

        template<typename TResourceType>
        static EditableRef<TResourceType> GetEditableResource(std::string const& fileName);

        template <typename TResourceType>
		static const std::string* FindKeyByVal(const ResourceRef<TResourceType>& resource);

		ENGINE_API static void		Unload(std::string const& fileName);
		ENGINE_API static void		UnloadAll(void);
		ENGINE_API static void		ShutDown(void);


	private:
									ResourceManager(void) = default;
									ResourceManager(ResourceManager const& rManager) = delete;
		ResourceManager&			operator=(ResourceManager const& rManager) = delete;

		ENGINE_API static ResourceManager*	GetInstance(void);
		ENGINE_API static bool				HasResource(std::string const& fileName);

		ENGINE_API
        static std::mutex			m_mutex;
        static ResourceManager* m_instance;

		std::unordered_map<std::string, ResourceContainer> m_resources;
	};

	// Template function definitions
	template<typename TResourceType>
	inline void engine::ResourceManager::Load(std::string const& fileName, bool absolute)
	{
        TResourceType* newVal = nullptr;

        {
            std::lock_guard lock(m_mutex);

            // Check if resource exists
            if (HasResource(fileName))
            {
                std::printf("Resource '%s' already loaded\n", fileName.c_str());
                return;
            }

            newVal = new TResourceType();
            GetInstance()->m_resources[fileName] = ResourceContainer(newVal, fileName);
        }

        std::string resourcePath;
        
        if (absolute)
            resourcePath = fileName;
        else if (Engine::HasEditor())
        {
            resourcePath = Engine::GetEngine()->GetProjectDir().string();
            resourcePath.push_back('\\');
            resourcePath += fileName;
        }
        else
            resourcePath = "assets\\" + fileName;
        

        if (!newVal->LoadResource(resourcePath.c_str()))
        {
            m_mutex.lock();
            GetInstance()->m_resources.erase(fileName);
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
        GetInstance()->m_resources[name] = ResourceContainer(newVal, name);
    }

    template<typename TResourceType>
	inline ResourceRef<TResourceType> engine::ResourceManager::GetResource(std::string const& fileName)
	{
        return static_cast<ResourceRef<TResourceType>>(GetEditableResource<TResourceType>(fileName));
	}

    template<typename TResourceType>
    inline EditableRef<TResourceType> ResourceManager::GetEditableResource(std::string const& fileName)
    {
        // Check if resource exists
        if (!HasResource(fileName))
            return {};

       ResourceContainer& container = GetInstance()->m_resources[fileName];
       TResourceType* resource = dynamic_cast<TResourceType*>(container.GetResource());

        if constexpr (!IsLoadedAsync<TResourceType>::m_value)
            return EditableRef<TResourceType>(&container, resource);

        else
        {

            if (!resource->HasFailedToLoad())
                return EditableRef<TResourceType>(&container, resource);

            Unload(fileName);
            return {};
        }
    }

    template <typename TResourceType> inline
    const std::string* ResourceManager::FindKeyByVal(const ResourceRef<TResourceType>& resource)
    {
        if (!resource)
            return nullptr;

        for (auto const& data : GetInstance()->m_resources)
        {
            if (resource == dynamic_cast<const TResourceType*>(data.second.GetResource()))
                return &data.first;
        }

        return nullptr;
    }

}