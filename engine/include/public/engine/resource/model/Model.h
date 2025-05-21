#pragma once

#include "engine/EngineExport.h"
#include "engine/resource/Resource.h"
#include "engine/resource/model/DynamicMesh.h"
#include "engine/resource/model/Mesh.h"

#include <vector>

namespace engine
{
    class Model final : public IResource
    {
        
    public:
        ENGINE_API		Model(void) = default;
        ENGINE_API      ~Model(void);

        ENGINE_API bool LoadResource(const char* fileName) override;

        ENGINE_API bool CanRender(void) const;
        ENGINE_API bool HasFailedToLoad(void) const;

        ENGINE_API bool IsDynamic(void) const;

        ENGINE_API void Draw(void) const;
        ENGINE_API void Draw(const std::vector<const MeshMaterial*>& materials) const;

        ENGINE_API uint32 GetMeshCount(void) const;

        ENGINE_API std::string GetName(void) const;

        // This function is meant to be used by renderer components, and ONLY from the
        // render thread.
        ENGINE_API
        const std::vector<Mesh>& GetStaticMeshes(void) const;

        ENGINE_API
        const std::vector<DynamicMesh>& GetDynamicMeshes(void) const;

    private:

        void ProcessTextures(const void* scene);
        std::string GetDirectory(const std::string& modelPath);
             
        void ProcessMeshes(const void* scene, const void* node, const std::string& name);
        void WorkerThreadLoad(const std::string& name);
        void RenderThreadSetup(void);

        std::vector<DynamicMesh>      m_dynamicMeshes;
        std::vector<Mesh>			  m_staticMeshes;
        std::string                   m_modelName;
        uint8						  m_loadStatus = 0;
        bool                          m_isDynamic = false;
    };


    template <>
    struct IsLoadedAsync<Model>
    {
        static constexpr bool m_value = true;
    };

}